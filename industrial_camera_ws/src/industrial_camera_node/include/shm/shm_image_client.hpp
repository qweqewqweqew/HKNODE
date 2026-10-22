#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "shm/interprocess.hpp"
#include "shm/shm_layout.hpp"

namespace industrial_camera {

// Header-only consumer. Zero-copy view stays valid only while FrameGuard is
// alive and StillValid() remains true. A force-reclaim invalidates the view.
class ShmImageClient {
 public:
  class FrameGuard {
   public:
    FrameGuard() = default;
    ~FrameGuard();

    FrameGuard(const FrameGuard&) = delete;
    FrameGuard& operator=(const FrameGuard&) = delete;
    FrameGuard(FrameGuard&& other) noexcept;
    FrameGuard& operator=(FrameGuard&& other) noexcept;

    bool valid() const { return slot_ != nullptr; }
    bool StillValid() const;
    const uint8_t* data() const { return data_; }
    uint32_t size() const { return size_; }
    uint64_t frame_id() const { return frame_id_; }
    uint64_t trigger_seq_id() const { return trigger_seq_id_; }
    uint32_t param_version_seq() const { return param_version_seq_; }

   private:
    friend class ShmImageClient;
    ShmSlotHeader* slot_{nullptr};
    const uint8_t* data_{nullptr};
    uint32_t size_{0};
    uint64_t frame_id_{0};
    uint64_t trigger_seq_id_{0};
    uint32_t param_version_seq_{0};

    void Release() noexcept;
  };

  bool Open(const std::string& shm_name, std::string* error);
  void Close();
  bool IsOpen() const { return base_ != nullptr; }

  FrameGuard Lock(uint32_t slot_index, uint64_t expected_frame_id,
                  uint64_t expected_trigger_seq_id, std::string* error) const;

  const ShmControlBlock* Control() const {
    return reinterpret_cast<const ShmControlBlock*>(base_);
  }

 private:
  bool Handshake(std::string* error) const;

  std::unique_ptr<boost::interprocess::shared_memory_object> shm_;
  std::unique_ptr<boost::interprocess::mapped_region> region_;
  uint8_t* base_{nullptr};
  std::size_t region_size_{0};
};

inline ShmImageClient::FrameGuard::~FrameGuard() { Release(); }

inline ShmImageClient::FrameGuard::FrameGuard(FrameGuard&& other) noexcept {
  *this = std::move(other);
}

inline ShmImageClient::FrameGuard& ShmImageClient::FrameGuard::operator=(
    FrameGuard&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  Release();
  slot_ = other.slot_;
  data_ = other.data_;
  size_ = other.size_;
  frame_id_ = other.frame_id_;
  trigger_seq_id_ = other.trigger_seq_id_;
  param_version_seq_ = other.param_version_seq_;
  other.slot_ = nullptr;
  other.data_ = nullptr;
  other.size_ = 0;
  return *this;
}

inline void ShmImageClient::FrameGuard::Release() noexcept {
  if (slot_ != nullptr) {
    slot_->ref_count.fetch_sub(1, std::memory_order_acq_rel);
    slot_ = nullptr;
  }
}

inline bool ShmImageClient::FrameGuard::StillValid() const {
  if (slot_ == nullptr) {
    return false;
  }
  const int32_t state = slot_->state.load(std::memory_order_acquire);
  const uint64_t fid = slot_->frame_id.load(std::memory_order_acquire);
  return state == static_cast<int32_t>(SlotState::kReady) && fid == frame_id_;
}

inline bool ShmImageClient::Open(const std::string& shm_name, std::string* error) {
  Close();
  try {
    shm_ = std::make_unique<boost::interprocess::shared_memory_object>(
        boost::interprocess::open_only, shm_name.c_str(),
        boost::interprocess::read_write);
    region_ = std::make_unique<boost::interprocess::mapped_region>(
        *shm_, boost::interprocess::read_write);
  } catch (const std::exception& ex) {
    if (error != nullptr) {
      *error = std::string("打开共享内存失败: ") + ex.what();
    }
    Close();
    return false;
  }
  base_ = static_cast<uint8_t*>(region_->get_address());
  region_size_ = region_->get_size();
  if (!Handshake(error)) {
    Close();
    return false;
  }
  return true;
}

inline void ShmImageClient::Close() {
  region_.reset();
  shm_.reset();
  base_ = nullptr;
  region_size_ = 0;
}

inline bool ShmImageClient::Handshake(std::string* error) const {
  auto fail = [&](const std::string& text) {
    if (error != nullptr) {
      *error = text;
    }
    return false;
  };
  if (base_ == nullptr || region_size_ < 64) {
    return fail("共享内存映射小于 64 字节");
  }
  const auto* ctrl = reinterpret_cast<const ShmControlBlock*>(base_);
  if (ctrl->magic_num != kShmMagic) {
    return fail("共享内存 magic 不匹配");
  }
  if (ctrl->version != kShmProtocolVersion) {
    return fail("共享内存协议版本不是 4");
  }
  if (ctrl->init_ready_flag.load(std::memory_order_acquire) != 1U) {
    return fail("共享内存尚未初始化完成");
  }
  if (region_size_ < ctrl->total_shm_size) {
    return fail("共享内存映射小于 total_shm_size");
  }
  return true;
}

inline ShmImageClient::FrameGuard ShmImageClient::Lock(uint32_t slot_index,
                                                       uint64_t expected_frame_id,
                                                       uint64_t expected_trigger_seq_id,
                                                       std::string* error) const {
  FrameGuard guard;
  auto fail = [&](const std::string& text) {
    if (error != nullptr) {
      *error = text;
    }
    return FrameGuard{};
  };
  if (!Handshake(error)) {
    return FrameGuard{};
  }
  const auto* ctrl = reinterpret_cast<const ShmControlBlock*>(base_);
  if (slot_index >= ctrl->slot_count) {
    return fail("槽位索引越界");
  }
  auto* slot = getSlotHeader(base_, slot_index);
  const int32_t state = slot->state.load(std::memory_order_acquire);
  const uint64_t fid = slot->frame_id.load(std::memory_order_acquire);
  if (state != static_cast<int32_t>(SlotState::kReady) || fid == 0 ||
      fid != expected_frame_id) {
    return fail("槽位未就绪或 frame_id 不一致");
  }
  if (slot->trigger_seq_id != expected_trigger_seq_id) {
    return fail("trigger_seq_id 与通知不一致");
  }
  slot->ref_count.fetch_add(1, std::memory_order_acq_rel);
  const int32_t state_after = slot->state.load(std::memory_order_acquire);
  const uint64_t fid_after = slot->frame_id.load(std::memory_order_acquire);
  if (state_after != static_cast<int32_t>(SlotState::kReady) || fid_after != fid ||
      slot->trigger_seq_id != expected_trigger_seq_id) {
    slot->ref_count.fetch_sub(1, std::memory_order_acq_rel);
    return fail("钉住槽位后校验失败，写入方已接管");
  }
  guard.slot_ = slot;
  guard.data_ = getSlotPayloadPtr(base_, slot);
  guard.size_ = slot->data_size;
  guard.frame_id_ = fid;
  guard.trigger_seq_id_ = slot->trigger_seq_id;
  guard.param_version_seq_ = slot->param_version_seq;
  if (!guard.StillValid()) {
    guard.Release();
    return fail("读取前帧号复核失败");
  }
  return guard;
}

}  // namespace industrial_camera
