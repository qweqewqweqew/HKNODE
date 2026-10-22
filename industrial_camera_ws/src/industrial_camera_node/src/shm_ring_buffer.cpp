#include "shm/shm_ring_buffer.hpp"

#include "shm/shm_layout.hpp"
#include "shm/interprocess.hpp"

#include <chrono>
#include <cstring>
#include <memory>
#include <thread>
#include <utility>

namespace industrial_camera {
namespace {

uint64_t ThisThreadToken() {
  return static_cast<uint64_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
}

uint64_t SteadyNowNs() {
  return static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::steady_clock::now().time_since_epoch())
          .count());
}

}  // namespace

struct ShmRingBuffer::BoostStorage {
  std::unique_ptr<boost::interprocess::shared_memory_object> shm;
  std::unique_ptr<boost::interprocess::mapped_region> region;
};

ShmRingBuffer::~ShmRingBuffer() { Reset(); }

void ShmRingBuffer::Reset() {
  delete storage_;
  storage_ = nullptr;
  base_ = nullptr;
  slot_count_ = 0;
  writer_bound_ = false;
  writer_token_ = 0;
  next_slot_ = 0;
  full_drop_ = 0;
  force_reclaim_ = 0;
}

bool ShmRingBuffer::Create(const std::string& shm_name, uint32_t slot_count,
                           uint64_t slot_capacity, uint64_t lock_timeout_ns,
                           std::string* error) {
  Reset();
  auto fail = [&](const std::string& text) {
    if (error != nullptr) {
      *error = text;
    }
    Reset();
    return false;
  };
  if (shm_name.empty() || shm_name.find('\\') != std::string::npos ||
      shm_name.find('/') != std::string::npos) {
    return fail("shm_name 为空或包含斜杠");
  }
  if (slot_count < 2 || slot_capacity == 0) {
    return fail("槽位数量或容量非法");
  }

  try {
    boost::interprocess::shared_memory_object::remove(shm_name.c_str());
    auto shm = std::make_unique<boost::interprocess::shared_memory_object>(
        boost::interprocess::create_only, shm_name.c_str(),
        boost::interprocess::read_write);
    const uint64_t total = ShmTotalBytes(slot_count, slot_capacity);
    shm->truncate(static_cast<boost::interprocess::offset_t>(total));
    auto region = std::make_unique<boost::interprocess::mapped_region>(
        *shm, boost::interprocess::read_write);
    if (region->get_size() < total) {
      return fail("共享内存映射尺寸不足");
    }
    auto* base = static_cast<uint8_t*>(region->get_address());
    std::memset(base, 0, static_cast<std::size_t>(total));
    auto* ctrl = reinterpret_cast<ShmControlBlock*>(base);
    ctrl->magic_num = kShmMagic;
    ctrl->version = kShmProtocolVersion;
    ctrl->slot_count = slot_count;
    ctrl->slot_capacity = slot_capacity;
    ctrl->total_shm_size = total;
    ctrl->slots_header_offset = 64;
    ctrl->payload_base_offset = 64ULL + static_cast<uint64_t>(slot_count) * 64ULL;
    ctrl->working_mode = 0;
    ctrl->init_ready_flag.store(0, std::memory_order_relaxed);
    ctrl->latest_slot_index.store(0, std::memory_order_relaxed);
    ctrl->total_written_frames.store(0, std::memory_order_relaxed);
    for (uint32_t i = 0; i < slot_count; ++i) {
      auto* slot = getSlotHeader(base, i);
      slot->frame_id.store(0, std::memory_order_relaxed);
      slot->ref_count.store(0, std::memory_order_relaxed);
      slot->state.store(static_cast<int32_t>(SlotState::kFree), std::memory_order_relaxed);
      slot->data_offset =
          ctrl->payload_base_offset + static_cast<uint64_t>(i) * slot_capacity;
    }
    ctrl->init_ready_flag.store(1, std::memory_order_release);

    storage_ = new BoostStorage();
    storage_->shm = std::move(shm);
    storage_->region = std::move(region);
    base_ = base;
  } catch (const std::exception& ex) {
    return fail(std::string("创建共享内存失败: ") + ex.what());
  }

  name_ = shm_name;
  slot_count_ = slot_count;
  slot_capacity_ = slot_capacity;
  lock_timeout_ns_ = lock_timeout_ns;
  return true;
}

void ShmRingBuffer::SetWorkingMode(uint32_t mode) {
  if (base_ == nullptr) {
    return;
  }
  reinterpret_cast<ShmControlBlock*>(base_)->working_mode = mode;
}

ShmWriteResult ShmRingBuffer::TryWrite(const ShmWriteRequest& request) {
  ShmWriteResult result;
  if (base_ == nullptr) {
    result.status = ShmWriteStatus::kNotReady;
    return result;
  }
  const uint64_t token = ThisThreadToken();
  if (!writer_bound_) {
    writer_bound_ = true;
    writer_token_ = token;
  } else if (writer_token_ != token) {
    result.status = ShmWriteStatus::kWrongThread;
    return result;
  }
  if (request.data == nullptr || request.size == 0 || request.size > slot_capacity_) {
    result.status = ShmWriteStatus::kTooLarge;
    return result;
  }

  const auto started = std::chrono::steady_clock::now();
  const uint64_t now = request.timestamp_ns == 0 ? SteadyNowNs() : request.timestamp_ns;
  int claim = -1;
  int reclaim_index = -1;
  uint64_t oldest_age = 0;

  for (uint32_t n = 0; n < slot_count_; ++n) {
    const uint32_t idx = (next_slot_ + n) % slot_count_;
    auto* slot = getSlotHeader(base_, idx);
    const int32_t refs = slot->ref_count.load(std::memory_order_acquire);
    const int32_t state = slot->state.load(std::memory_order_acquire);
    const bool free_state = state == static_cast<int32_t>(SlotState::kFree) ||
                            state == static_cast<int32_t>(SlotState::kReady) ||
                            state == static_cast<int32_t>(SlotState::kWriting);
    if (refs == 0 && free_state) {
      slot->state.store(static_cast<int32_t>(SlotState::kWriting), std::memory_order_release);
      if (slot->ref_count.load(std::memory_order_acquire) == 0) {
        claim = static_cast<int>(idx);
        break;
      }
      continue;
    }
    if (refs > 0 && slot->timestamp_ns > 0 && now >= slot->timestamp_ns) {
      const uint64_t age = now - slot->timestamp_ns;
      if (age >= lock_timeout_ns_ && age >= oldest_age) {
        oldest_age = age;
        reclaim_index = static_cast<int>(idx);
      }
    }
  }

  bool reclaimed = false;
  if (claim < 0 && reclaim_index >= 0) {
    auto* slot = getSlotHeader(base_, static_cast<uint32_t>(reclaim_index));
    slot->state.store(static_cast<int32_t>(SlotState::kWriting), std::memory_order_release);
    slot->ref_count.store(0, std::memory_order_release);
    claim = reclaim_index;
    reclaimed = true;
    ++force_reclaim_;
  }
  if (claim < 0) {
    ++full_drop_;
    result.status = ShmWriteStatus::kFullDrop;
    return result;
  }

  auto* slot = getSlotHeader(base_, static_cast<uint32_t>(claim));
  std::memcpy(base_ + slot->data_offset, request.data, request.size);
  slot->timestamp_ns = now;
  slot->width = request.width;
  slot->height = request.height;
  slot->step = request.step;
  slot->pixel_format = request.pixel_format;
  slot->data_size = request.size;
  slot->param_version_seq = request.param_version_seq;
  slot->trigger_seq_id = request.trigger_seq_id;
  slot->frame_id.store(request.frame_id, std::memory_order_release);
  slot->state.store(static_cast<int32_t>(SlotState::kReady), std::memory_order_release);

  auto* ctrl = reinterpret_cast<ShmControlBlock*>(base_);
  ctrl->latest_slot_index.store(static_cast<uint32_t>(claim), std::memory_order_release);
  ctrl->total_written_frames.fetch_add(1, std::memory_order_relaxed);
  next_slot_ = (static_cast<uint32_t>(claim) + 1U) % slot_count_;

  uint32_t active = 0;
  for (uint32_t i = 0; i < slot_count_; ++i) {
    if (getSlotHeader(base_, i)->ref_count.load(std::memory_order_acquire) > 0) {
      ++active;
    }
  }
  const auto elapsed = std::chrono::steady_clock::now() - started;
  result.status = reclaimed ? ShmWriteStatus::kForceReclaim : ShmWriteStatus::kOk;
  result.slot_index = static_cast<uint32_t>(claim);
  result.data_offset = slot->data_offset;
  result.write_ns = static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count());
  result.active_slots = active;
  return result;
}

}  // namespace industrial_camera
