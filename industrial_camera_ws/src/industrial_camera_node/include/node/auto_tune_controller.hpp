#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <optional>

#include "common/types.hpp"

namespace industrial_camera {

class AutoTuneController {
 public:
  explicit AutoTuneController(std::atomic<uint32_t>& version) : version_(version) {}

  // Returns false when the request is stale and must be dropped.
  bool Accept(const TuneRequest& request);
  std::optional<TuneRequest> Take();
  uint64_t SkipCount() const { return skip_.load(std::memory_order_relaxed); }

 private:
  std::atomic<uint32_t>& version_;
  std::atomic<uint64_t> skip_{0};
  std::mutex mutex_;
  std::optional<TuneRequest> pending_;
};

}  // namespace industrial_camera
