#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "common/camera_logger.hpp"
#include "common/types.hpp"
#include "hal/i_camera_device.hpp"
#include "node/auto_tune_controller.hpp"
#include "node/trigger_token_queue.hpp"
#include "shm/shm_ring_buffer.hpp"

namespace industrial_camera {

struct HealthView {
  uint64_t sdk_net_drop{0};
  uint64_t hw_frame_gap{0};
  uint64_t shm_full_drop{0};
  uint64_t shm_force_reclaim{0};
  uint64_t trigger_timeout_drop{0};
  uint64_t reconnect_drop{0};
  uint64_t orphan_drop{0};
  uint64_t trigger_ok{0};
  uint64_t tune_apply{0};
  uint64_t tune_skip{0};
  uint32_t param_version{0};
};

class CameraStationWorker {
 public:
  using Notifier = std::function<void(const FrameNotice&)>;

  CameraStationWorker(StationConfig config, std::unique_ptr<ICameraDevice> device,
                      CameraLogger& logger);
  ~CameraStationWorker();

  CameraStationWorker(const CameraStationWorker&) = delete;
  CameraStationWorker& operator=(const CameraStationWorker&) = delete;

  bool Start(std::string* error);
  void Stop();
  bool SubmitTrigger(const TriggerRequest& request, std::string* error);
  bool SubmitTune(const TuneRequest& request, std::string* error);
  ParamWriteResult SetParam(const ParamWriteRequest& request);
  void SetNotifier(Notifier notifier);

  HealthView Health() const;
  std::string FormatStat();
  uint8_t* ShmBase() const { return shm_.Base(); }
  const StationConfig& Config() const { return config_; }
  uint32_t ParamVersion() const { return version_.load(std::memory_order_acquire); }

 private:
  void Loop();
  void OnImage(const RawFrame& frame);
  void OnException(int code);
  bool ApplyFeatures(std::string* error);
  bool ApplyMode(std::string* error);
  bool CheckPayload(std::string* error);
  bool RegisterCallbacks(std::string* error);
  bool DisableAutos(std::string* error);
  bool WriteExposureGain(bool write_exposure, double exposure, bool write_gain, double gain,
                         bool* changed, std::string* error);
  void ApplyTrigger(const TriggerRequest& request);
  void ApplyTune(const TuneRequest& request);
  void HandleReconnect();
  void PublishPending();
  void MaybeStat();
  void PushNotice(FrameNotice notice);
  uint64_t PeriodOf(std::atomic<uint64_t>* value, std::atomic<uint64_t>* mark);

  StationConfig config_;
  std::unique_ptr<ICameraDevice> device_;
  CameraLogger& logger_;
  ShmRingBuffer shm_;
  TriggerTokenQueue tokens_;
  std::atomic<uint32_t> version_{1};
  AutoTuneController tuner_;
  std::recursive_mutex sdk_mutex_;
  std::mutex cmd_mutex_;
  std::mutex notice_mutex_;
  std::mutex notifier_mutex_;
  std::condition_variable cmd_cv_;
  std::thread thread_;
  std::vector<TriggerRequest> pending_triggers_;
  std::vector<FrameNotice> notices_;
  Notifier notifier_;

  std::atomic<bool> stop_{false};
  std::atomic<bool> streaming_{false};
  std::atomic<bool> reconnect_requested_{false};
  std::atomic<bool> notice_pending_{false};
  std::atomic<bool> tune_pending_{false};
  std::atomic<bool> trigger_mode_{false};
  std::atomic<bool> software_source_{false};
  std::atomic<bool> exempt_gap_{true};
  std::atomic<uint64_t> last_frame_id_{0};
  std::atomic<uint32_t> exposure_bits_{0};
  std::atomic<uint32_t> gain_bits_{0};

  std::atomic<uint64_t> sdk_net_drop_{0};
  std::atomic<uint64_t> hw_frame_gap_{0};
  std::atomic<uint64_t> shm_full_drop_{0};
  std::atomic<uint64_t> shm_force_reclaim_{0};
  std::atomic<uint64_t> trigger_timeout_drop_{0};
  std::atomic<uint64_t> reconnect_drop_{0};
  std::atomic<uint64_t> orphan_drop_{0};
  std::atomic<uint64_t> trigger_ok_{0};
  std::atomic<uint64_t> tune_apply_{0};
  std::atomic<uint64_t> tune_skip_{0};
  std::atomic<uint64_t> write_ns_sum_{0};
  std::atomic<uint64_t> write_count_{0};
  std::atomic<uint32_t> active_peak_{0};

  std::atomic<uint64_t> mark_sdk_{0};
  std::atomic<uint64_t> mark_gap_{0};
  std::atomic<uint64_t> mark_full_{0};
  std::atomic<uint64_t> mark_reclaim_{0};
  std::atomic<uint64_t> mark_timeout_{0};
  std::atomic<uint64_t> mark_reconnect_{0};
  std::atomic<uint64_t> mark_orphan_{0};
  std::atomic<uint64_t> mark_trigger_{0};
  std::atomic<uint64_t> mark_tune_{0};
  std::atomic<uint64_t> mark_skip_{0};
  std::atomic<uint64_t> mark_write_ns_{0};
  std::atomic<uint64_t> mark_write_count_{0};
  mutable std::atomic<int64_t> last_stat_ns_{0};
  bool thread_started_{false};
};

}  // namespace industrial_camera
