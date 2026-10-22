#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

#include "common/types.hpp"

namespace industrial_camera {

static_assert(std::atomic<uint64_t>::is_always_lock_free,
              "FATAL: uint64_t atomic must be lock-free");
static_assert(std::atomic<int32_t>::is_always_lock_free,
              "FATAL: int32_t atomic must be lock-free");
static_assert(std::atomic<uint32_t>::is_always_lock_free,
              "FATAL: uint32_t atomic must be lock-free");

enum class SlotState : int32_t {
  kFree = 0,
  kWriting = 1,
  kReady = 2
};

#pragma pack(push, 8)

struct ShmSlotHeader {
  std::atomic<uint64_t> frame_id;
  std::atomic<int32_t> ref_count;
  std::atomic<int32_t> state;
  uint64_t timestamp_ns;
  uint32_t width;
  uint32_t height;
  uint32_t step;
  uint32_t pixel_format;
  uint32_t data_size;
  uint32_t param_version_seq;
  uint64_t data_offset;
  uint64_t trigger_seq_id;
};

struct ShmControlBlock {
  uint32_t magic_num;
  uint32_t version;
  std::atomic<uint32_t> init_ready_flag;
  uint32_t slot_count;
  uint64_t slot_capacity;
  uint64_t total_shm_size;
  uint64_t slots_header_offset;
  uint64_t payload_base_offset;
  std::atomic<uint32_t> latest_slot_index;
  uint32_t working_mode;
  std::atomic<uint64_t> total_written_frames;
};

#pragma pack(pop)

static_assert(sizeof(ShmSlotHeader) == 64, "ShmSlotHeader must be exactly 64 bytes");
static_assert(sizeof(ShmControlBlock) == 64, "ShmControlBlock must be exactly 64 bytes");
static_assert(offsetof(ShmSlotHeader, frame_id) == 0, "frame_id offset");
static_assert(offsetof(ShmSlotHeader, ref_count) == 8, "ref_count offset");
static_assert(offsetof(ShmSlotHeader, state) == 12, "state offset");
static_assert(offsetof(ShmSlotHeader, timestamp_ns) == 16, "timestamp offset");
static_assert(offsetof(ShmSlotHeader, width) == 24, "width offset");
static_assert(offsetof(ShmSlotHeader, data_offset) == 48, "data_offset offset");
static_assert(offsetof(ShmSlotHeader, trigger_seq_id) == 56, "trigger_seq_id offset");
static_assert(offsetof(ShmControlBlock, init_ready_flag) == 8, "init_ready offset");
static_assert(offsetof(ShmControlBlock, total_written_frames) == 56,
              "total_written_frames offset");

inline ShmSlotHeader* getSlotHeader(uint8_t* shm_base, uint32_t slot_idx) noexcept {
  auto* ctrl = reinterpret_cast<ShmControlBlock*>(shm_base);
  return reinterpret_cast<ShmSlotHeader*>(shm_base + ctrl->slots_header_offset) + slot_idx;
}

inline const uint8_t* getSlotPayloadPtr(const uint8_t* shm_base,
                                        const ShmSlotHeader* slot) noexcept {
  return shm_base + slot->data_offset;
}

inline uint64_t ShmTotalBytes(uint32_t slot_count, uint64_t slot_capacity) {
  return 64ULL + static_cast<uint64_t>(slot_count) * 64ULL +
         static_cast<uint64_t>(slot_count) * slot_capacity;
}

}  // namespace industrial_camera
