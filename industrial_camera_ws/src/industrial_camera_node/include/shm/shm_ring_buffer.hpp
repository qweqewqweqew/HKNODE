#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace industrial_camera {

enum class ShmWriteStatus {
  kOk,
  kFullDrop,
  kForceReclaim,
  kTooLarge,
  kWrongThread,
  kNotReady
};

struct ShmWriteRequest {
  const uint8_t* data{nullptr};
  uint32_t size{0};
  uint32_t width{0};
  uint32_t height{0};
  uint32_t step{0};
  uint32_t pixel_format{0};
  uint64_t frame_id{0};
  uint32_t param_version_seq{0};
  uint64_t trigger_seq_id{0};
  uint64_t timestamp_ns{0};
};

struct ShmWriteResult {
  ShmWriteStatus status{ShmWriteStatus::kNotReady};
  uint32_t slot_index{0};
  uint64_t data_offset{0};
  uint64_t write_ns{0};
  uint32_t active_slots{0};
};

class ShmRingBuffer {
 public:
  ShmRingBuffer() = default;
  ~ShmRingBuffer();

  ShmRingBuffer(const ShmRingBuffer&) = delete;
  ShmRingBuffer& operator=(const ShmRingBuffer&) = delete;

  bool Create(const std::string& shm_name, uint32_t slot_count, uint64_t slot_capacity,
              uint64_t lock_timeout_ns, std::string* error);
  void Reset();

  ShmWriteResult TryWrite(const ShmWriteRequest& request);
  void SetWorkingMode(uint32_t mode);

  uint8_t* Base() const { return base_; }
  const std::string& Name() const { return name_; }
  uint64_t FullDropCount() const { return full_drop_; }
  uint64_t ForceReclaimCount() const { return force_reclaim_; }
  uint32_t SlotCount() const { return slot_count_; }

 private:
  std::string name_;
  uint32_t slot_count_{0};
  uint64_t slot_capacity_{0};
  uint64_t lock_timeout_ns_{0};
  uint32_t next_slot_{0};
  uint64_t full_drop_{0};
  uint64_t force_reclaim_{0};
  bool writer_bound_{false};
  uint64_t writer_token_{0};
  uint8_t* base_{nullptr};

  struct BoostStorage;
  BoostStorage* storage_{nullptr};
};

}  // namespace industrial_camera
