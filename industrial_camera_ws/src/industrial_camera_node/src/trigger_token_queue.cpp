#include "node/trigger_token_queue.hpp"

namespace industrial_camera {

TriggerTokenQueue::TriggerTokenQueue(std::size_t max_depth) : max_depth_(max_depth == 0 ? 1 : max_depth) {}

bool TriggerTokenQueue::TryPush(TriggerToken token) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (queue_.size() >= max_depth_) {
    return false;
  }
  queue_.push_back(std::move(token));
  return true;
}

bool TriggerTokenQueue::TryPop(TriggerToken* token) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (queue_.empty() || token == nullptr) {
    return false;
  }
  *token = std::move(queue_.front());
  queue_.pop_front();
  return true;
}

bool TriggerTokenQueue::RemoveSeq(uint64_t trigger_seq_id) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (queue_.empty() || queue_.front().trigger_seq_id != trigger_seq_id) {
    return false;
  }
  queue_.pop_front();
  return true;
}

int TriggerTokenQueue::ExpireOlderThan(uint64_t now_ns, uint64_t timeout_ns) {
  std::lock_guard<std::mutex> lock(mutex_);
  int removed = 0;
  while (!queue_.empty() && now_ns >= queue_.front().enqueue_time_ns &&
         now_ns - queue_.front().enqueue_time_ns > timeout_ns) {
    queue_.pop_front();
    ++removed;
  }
  return removed;
}

int TriggerTokenQueue::Clear() {
  std::lock_guard<std::mutex> lock(mutex_);
  const int removed = static_cast<int>(queue_.size());
  queue_.clear();
  return removed;
}

std::size_t TriggerTokenQueue::Size() const {
  std::lock_guard<std::mutex> lock(mutex_);
  return queue_.size();
}

}  // namespace industrial_camera
