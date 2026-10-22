#pragma once

#include <cstdint>
#include <deque>
#include <mutex>
#include <string>

namespace industrial_camera {

struct TriggerToken {
  uint64_t trigger_seq_id{0};
  std::string workpiece_id;
  uint64_t enqueue_time_ns{0};
  uint32_t param_version_seq{0};
};

class TriggerTokenQueue {
 public:
  explicit TriggerTokenQueue(std::size_t max_depth = 1);

  bool TryPush(TriggerToken token);
  bool TryPop(TriggerToken* token);
  bool RemoveSeq(uint64_t trigger_seq_id);
  int ExpireOlderThan(uint64_t now_ns, uint64_t timeout_ns);
  int Clear();
  std::size_t Size() const;

 private:
  std::size_t max_depth_;
  mutable std::mutex mutex_;
  std::deque<TriggerToken> queue_;
};

}  // namespace industrial_camera
