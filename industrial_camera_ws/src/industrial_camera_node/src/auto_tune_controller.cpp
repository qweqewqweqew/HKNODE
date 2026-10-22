#include "node/auto_tune_controller.hpp"

namespace industrial_camera {

bool AutoTuneController::Accept(const TuneRequest& request) {
  const uint32_t current = version_.load(std::memory_order_acquire);
  std::lock_guard<std::mutex> lock(mutex_);
  if (request.base_param_version < current) {
    skip_.fetch_add(1, std::memory_order_relaxed);
    return false;
  }
  if (pending_.has_value() &&
      request.base_param_version < pending_->base_param_version) {
    skip_.fetch_add(1, std::memory_order_relaxed);
    return false;
  }
  pending_ = request;
  return true;
}

std::optional<TuneRequest> AutoTuneController::Take() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!pending_.has_value()) {
    return std::nullopt;
  }
  TuneRequest task = std::move(*pending_);
  pending_.reset();
  return task;
}

}  // namespace industrial_camera
