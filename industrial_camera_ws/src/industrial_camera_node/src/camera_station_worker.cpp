#include "node/camera_station_worker.hpp"

#include "common/sdk_error.hpp"

#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <utility>

namespace industrial_camera {
namespace {

uint64_t SteadyNs() {
  return static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::steady_clock::now().time_since_epoch())
          .count());
}

int64_t SystemNs() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

double ClampValue(double value, double low, double high) {
  if (value < low) {
    return low;
  }
  if (value > high) {
    return high;
  }
  return value;
}

std::string SdkTriggerSource(const std::string& source) {
  if (source == "line0") {
    return "Line0";
  }
  if (source == "line1") {
    return "Line1";
  }
  if (source == "line2") {
    return "Line2";
  }
  if (source == "line3") {
    return "Line3";
  }
  return "Software";
}

}  // namespace

CameraStationWorker::CameraStationWorker(StationConfig config,
                                         std::unique_ptr<ICameraDevice> device,
                                         CameraLogger& logger)
    : config_(std::move(config)),
      device_(std::move(device)),
      logger_(logger),
      tokens_(static_cast<std::size_t>(config_.trigger_queue_depth)),
      tuner_(version_) {}

CameraStationWorker::~CameraStationWorker() { Stop(); }

bool CameraStationWorker::Start(std::string* error) {
  Stop();
  stop_.store(false);
  const uint64_t capacity =
      static_cast<uint64_t>(config_.shm_slot_capacity_mb) * 1024ULL * 1024ULL;
  if (!shm_.Create(config_.shm_name, static_cast<uint32_t>(config_.shm_slot_count), capacity,
                   static_cast<uint64_t>(config_.shm_lock_timeout_ms) * 1000000ULL, error)) {
    return false;
  }
  if (!device_->Open(config_.connect_type, config_.device_identity, error)) {
    shm_.Reset();
    return false;
  }
  if (!ApplyFeatures(error) || !ApplyMode(error) || !CheckPayload(error) ||
      !RegisterCallbacks(error) || !device_->StartGrabbing(error)) {
    std::string ignored;
    device_->StopGrabbing(&ignored);
    device_->Close();
    shm_.Reset();
    return false;
  }
  for (const auto& feature : config_.features) {
    try {
      if (feature.feature == "ExposureTime" && feature.type == ParamType::kFloat) {
        exposure_bits_.store(PackFloat(static_cast<float>(std::stod(feature.value))),
                             std::memory_order_release);
      }
      if (feature.feature == "Gain" && feature.type == ParamType::kFloat) {
        gain_bits_.store(PackFloat(static_cast<float>(std::stod(feature.value))),
                         std::memory_order_release);
      }
    } catch (const std::exception&) {
      if (error != nullptr) {
        *error = "曝光或增益初值无法解析";
      }
      Stop();
      return false;
    }
  }
  version_.store(1, std::memory_order_release);
  streaming_.store(true);
  exempt_gap_.store(true);
  last_stat_ns_.store(static_cast<int64_t>(SteadyNs()));
  thread_ = std::thread([this] { Loop(); });
  thread_started_ = true;
  logger_.Info(config_.name, "工位已开流");
  return true;
}

void CameraStationWorker::Stop() {
  stop_.store(true);
  streaming_.store(false);
  cmd_cv_.notify_all();
  if (thread_started_ && thread_.joinable()) {
    thread_.join();
  }
  thread_started_ = false;
  if (device_ != nullptr) {
    std::lock_guard<std::recursive_mutex> lock(sdk_mutex_);
    std::string ignored;
    device_->StopGrabbing(&ignored);
    device_->Close();
  }
  shm_.Reset();
}

bool CameraStationWorker::SubmitTrigger(const TriggerRequest& request, std::string* error) {
  if (!software_source_.load(std::memory_order_acquire)) {
    if (error != nullptr) {
      *error = "当前不是软触发模式，拒绝 TriggerCaptureCmd";
    }
    return false;
  }
  {
    std::lock_guard<std::mutex> lock(cmd_mutex_);
    if (static_cast<int>(pending_triggers_.size()) >= config_.trigger_queue_depth) {
      if (error != nullptr) {
        *error = "触发命令队列已满";
      }
      return false;
    }
    pending_triggers_.push_back(request);
  }
  cmd_cv_.notify_all();
  return true;
}

bool CameraStationWorker::SubmitTune(const TuneRequest& request, std::string* error) {
  if (!config_.auto_tune_enable) {
    tune_skip_.fetch_add(1, std::memory_order_relaxed);
    if (error != nullptr) {
      *error = "该工位未启用算法闭环调参";
    }
    return false;
  }
  if (!tuner_.Accept(request)) {
    tune_skip_.fetch_add(1, std::memory_order_relaxed);
    if (error != nullptr) {
      *error = "过期调参已丢弃";
    }
    return false;
  }
  tune_pending_.store(true);
  cmd_cv_.notify_all();
  return true;
}

ParamWriteResult CameraStationWorker::SetParam(const ParamWriteRequest& request) {
  ParamWriteResult result;
  result.new_param_version_seq = version_.load(std::memory_order_acquire);
  std::lock_guard<std::recursive_mutex> lock(sdk_mutex_);
  std::string error;
  if (request.key == "@WorkingMode") {
    if (request.value != "continuous" && request.value != "trigger") {
      result.message = "WorkingMode 只能是 continuous 或 trigger";
      return result;
    }
    streaming_.store(false);
    device_->StopGrabbing(&error);
    const int dropped = tokens_.Clear();
    trigger_timeout_drop_.fetch_add(static_cast<uint64_t>(dropped), std::memory_order_relaxed);
    config_.working_mode = request.value;
    if (!ApplyMode(&error) || !device_->StartGrabbing(&error)) {
      result.sdk_error_code = device_->LastSdkCode();
      result.message = "切换工作模式失败: " + error;
      return result;
    }
    streaming_.store(true);
    result.success = true;
    result.new_param_version_seq = version_.load(std::memory_order_acquire);
    result.message = "工作模式已切换，顺序为停流、改参、再开流";
    logger_.Info(config_.name, result.message + " -> " + request.value);
    return result;
  }

  bool ok = false;
  try {
    switch (request.type) {
      case ParamType::kInt:
        ok = device_->SetInt(request.key, std::stoll(request.value), &error);
        break;
      case ParamType::kFloat: {
        double value = std::stod(request.value);
        if (request.key == "ExposureTime" || request.key == "Gain") {
          if (!DisableAutos(&error)) {
            result.message = error;
            result.sdk_error_code = device_->LastSdkCode();
            return result;
          }
          const bool exposure = request.key == "ExposureTime";
          const double clamped =
              exposure ? ClampValue(value, config_.auto_tune_exposure_min_us,
                                     config_.auto_tune_exposure_max_us)
                       : ClampValue(value, config_.auto_tune_gain_min_db,
                                    config_.auto_tune_gain_max_db);
          bool changed = false;
          ok = WriteExposureGain(exposure, clamped, !exposure, clamped, &changed, &error);
          if (ok && std::abs(clamped - value) > 1e-6) {
            error = "已按安全范围钳位到 [" +
                    std::to_string(exposure ? config_.auto_tune_exposure_min_us
                                            : config_.auto_tune_gain_min_db) +
                    ", " +
                    std::to_string(exposure ? config_.auto_tune_exposure_max_us
                                            : config_.auto_tune_gain_max_db) +
                    "]";
          }
        } else {
          ok = device_->SetFloat(request.key, value, &error);
        }
        break;
      }
      case ParamType::kEnum:
        ok = device_->SetEnum(request.key, request.value, &error);
        break;
      case ParamType::kBool:
        ok = device_->SetBool(request.key,
                              request.value == "1" || request.value == "true" ||
                                  request.value == "True",
                              &error);
        break;
      case ParamType::kString:
        ok = device_->SetString(request.key, request.value, &error);
        break;
      case ParamType::kCmd:
        ok = device_->SetCommand(request.key, &error);
        break;
    }
  } catch (const std::exception&) {
    result.message = "参数值无法解析: " + request.value;
    return result;
  }
  result.success = ok;
  result.sdk_error_code = device_->LastSdkCode();
  result.new_param_version_seq = version_.load(std::memory_order_acquire);
  result.message = ok ? (error.empty() ? "修改成功" : error) : error;
  return result;
}

void CameraStationWorker::SetNotifier(Notifier notifier) {
  std::lock_guard<std::mutex> lock(notifier_mutex_);
  notifier_ = std::move(notifier);
}

HealthView CameraStationWorker::Health() const {
  HealthView view;
  view.sdk_net_drop = sdk_net_drop_.load();
  view.hw_frame_gap = hw_frame_gap_.load();
  view.shm_full_drop = shm_full_drop_.load();
  view.shm_force_reclaim = shm_force_reclaim_.load();
  view.trigger_timeout_drop = trigger_timeout_drop_.load();
  view.reconnect_drop = reconnect_drop_.load();
  view.orphan_drop = orphan_drop_.load();
  view.trigger_ok = trigger_ok_.load();
  view.tune_apply = tune_apply_.load();
  view.tune_skip = tune_skip_.load();
  view.param_version = version_.load();
  return view;
}

bool CameraStationWorker::ApplyFeatures(std::string* error) {
  for (const auto& feature : config_.features) {
    if (!device_->ApplyFeature(feature, error)) {
      return false;
    }
  }
  return true;
}

bool CameraStationWorker::ApplyMode(std::string* error) {
  if (!device_->SetEnum("AcquisitionMode", "Continuous", error)) {
    return false;
  }
  const bool trigger = config_.working_mode == "trigger";
  if (trigger) {
    if (!device_->SetEnum("TriggerMode", "On", error)) {
      return false;
    }
    if (!device_->SetEnum("TriggerSource", SdkTriggerSource(config_.trigger_source), error)) {
      return false;
    }
  } else if (!device_->SetEnum("TriggerMode", "Off", error)) {
    return false;
  }
  trigger_mode_.store(trigger);
  software_source_.store(trigger && config_.trigger_source == "software");
  shm_.SetWorkingMode(trigger ? 1U : 0U);
  return true;
}

bool CameraStationWorker::CheckPayload(std::string* error) {
  uint64_t payload = 0;
  if (!device_->GetPayloadSize(&payload, error)) {
    return false;
  }
  const uint64_t capacity =
      static_cast<uint64_t>(config_.shm_slot_capacity_mb) * 1024ULL * 1024ULL;
  if (payload == 0 || payload > capacity) {
    if (error != nullptr) {
      *error = "预检失败: PayloadSize " + std::to_string(payload) + " 超过槽容量 " +
               std::to_string(capacity);
    }
    return false;
  }
  return true;
}

bool CameraStationWorker::RegisterCallbacks(std::string* error) {
  if (!device_->RegisterImageCallback([this](const RawFrame& frame) { OnImage(frame); }, error)) {
    return false;
  }
  if (!device_->RegisterExceptionCallback([this](int code) { OnException(code); }, error)) {
    return false;
  }
  return true;
}

bool CameraStationWorker::DisableAutos(std::string* error) {
  return device_->SetEnum("ExposureAuto", "Off", error) &&
         device_->SetEnum("GainAuto", "Off", error);
}

bool CameraStationWorker::WriteExposureGain(bool write_exposure, double exposure, bool write_gain,
                                            double gain, bool* changed, std::string* error) {
  if (write_exposure) {
    const double clamped = ClampValue(exposure, config_.auto_tune_exposure_min_us,
                                      config_.auto_tune_exposure_max_us);
    if (!device_->SetFloat("ExposureTime", clamped, error)) {
      return false;
    }
    exposure_bits_.store(PackFloat(static_cast<float>(clamped)), std::memory_order_release);
    *changed = true;
  }
  if (write_gain) {
    const double clamped =
        ClampValue(gain, config_.auto_tune_gain_min_db, config_.auto_tune_gain_max_db);
    if (!device_->SetFloat("Gain", clamped, error)) {
      return false;
    }
    gain_bits_.store(PackFloat(static_cast<float>(clamped)), std::memory_order_release);
    *changed = true;
  }
  if (*changed) {
    version_.fetch_add(1, std::memory_order_acq_rel);
  }
  return true;
}

void CameraStationWorker::ApplyTrigger(const TriggerRequest& request) {
  std::lock_guard<std::recursive_mutex> lock(sdk_mutex_);
  if (!software_source_.load(std::memory_order_acquire)) {
    logger_.Warn(config_.name, "软触发命令在非软触发模式下被丢弃");
    return;
  }
  std::string error;
  bool changed = false;
  const bool write_exposure = request.override_exposure_us > 0.F;
  const bool write_gain = request.override_gain_db >= 0.F;
  if (write_exposure || write_gain) {
    if (!DisableAutos(&error) ||
        !WriteExposureGain(write_exposure, request.override_exposure_us, write_gain,
                           request.override_gain_db, &changed, &error)) {
      logger_.Error(config_.name, "一令一参写入失败: " + error);
      return;
    }
    if (config_.param_settle_us > 0) {
      std::this_thread::sleep_for(std::chrono::microseconds(config_.param_settle_us));
    }
  }
  TriggerToken token;
  token.trigger_seq_id = request.trigger_seq_id;
  token.workpiece_id = request.workpiece_id;
  token.enqueue_time_ns = SteadyNs();
  token.param_version_seq = version_.load(std::memory_order_acquire);
  if (!tokens_.TryPush(token)) {
    logger_.Error(config_.name, "待触发令牌队列已满，丢弃本次软触发");
    return;
  }
  if (!device_->SetCommand("TriggerSoftware", &error)) {
    tokens_.RemoveSeq(request.trigger_seq_id);
    logger_.Error(config_.name, "TriggerSoftware 失败: " + error);
  }
}

void CameraStationWorker::ApplyTune(const TuneRequest& request) {
  std::lock_guard<std::recursive_mutex> lock(sdk_mutex_);
  if (request.base_param_version < version_.load(std::memory_order_acquire)) {
    tune_skip_.fetch_add(1, std::memory_order_relaxed);
    logger_.Info(config_.name, "执行前再次丢弃过期调参");
    return;
  }
  std::string error;
  bool changed = false;
  if (request.tune_exposure || request.tune_gain) {
    if (!DisableAutos(&error) ||
        !WriteExposureGain(request.tune_exposure, request.target_exposure_us, request.tune_gain,
                           request.target_gain_db, &changed, &error)) {
      logger_.Error(config_.name, "闭环调参失败: " + error);
      return;
    }
  }
  if (changed) {
    tune_apply_.fetch_add(1, std::memory_order_relaxed);
  }
  if (!request.retrigger_immediately) {
    return;
  }
  if (!software_source_.load(std::memory_order_acquire)) {
    logger_.Warn(config_.name, "当前不是软触发，忽略立即重拍");
    return;
  }
  if (config_.param_settle_us > 0) {
    std::this_thread::sleep_for(std::chrono::microseconds(config_.param_settle_us));
  }
  TriggerToken token;
  token.trigger_seq_id = request.retrigger_seq_id;
  token.workpiece_id = request.workpiece_id;
  token.enqueue_time_ns = SteadyNs();
  token.param_version_seq = version_.load(std::memory_order_acquire);
  if (!tokens_.TryPush(std::move(token))) {
    logger_.Error(config_.name, "立即重拍时令牌队列已满");
    return;
  }
  if (!device_->SetCommand("TriggerSoftware", &error)) {
    tokens_.RemoveSeq(request.retrigger_seq_id);
    logger_.Error(config_.name, "立即重拍 TriggerSoftware 失败: " + error);
  }
}

void CameraStationWorker::HandleReconnect() {
  reconnect_requested_.store(false);
  streaming_.store(false);
  const int dropped = tokens_.Clear();
  reconnect_drop_.fetch_add(static_cast<uint64_t>(dropped), std::memory_order_relaxed);
  {
    std::lock_guard<std::recursive_mutex> lock(sdk_mutex_);
    std::string ignored;
    device_->StopGrabbing(&ignored);
    device_->Close();
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(config_.reconnect_interval_ms));
  if (stop_.load()) {
    return;
  }
  std::lock_guard<std::recursive_mutex> lock(sdk_mutex_);
  std::string error;
  const float exposure = UnpackFloat(exposure_bits_.load());
  const float gain = UnpackFloat(gain_bits_.load());
  if (!device_->Open(config_.connect_type, config_.device_identity, &error) ||
      !ApplyFeatures(&error) || !DisableAutos(&error) ||
      !device_->SetFloat("ExposureTime", exposure, &error) ||
      !device_->SetFloat("Gain", gain, &error) || !ApplyMode(&error) || !CheckPayload(&error) ||
      !RegisterCallbacks(&error) || !device_->StartGrabbing(&error)) {
    logger_.Error(config_.name, "重连失败，将继续重试: " + error);
    reconnect_requested_.store(true);
    return;
  }
  exempt_gap_.store(true);
  last_frame_id_.store(0);
  streaming_.store(true);
  logger_.Info(config_.name, "重连成功，共享内存保持原映射");
}

void CameraStationWorker::OnException(int code) {
  logger_.Error(config_.name, "收到相机异常回调，转入重连: " + TranslateSdkError(code));
  reconnect_requested_.store(true);
  cmd_cv_.notify_all();
}

void CameraStationWorker::OnImage(const RawFrame& frame) {
  if (!streaming_.load(std::memory_order_acquire) || frame.data == nullptr || frame.size == 0) {
    if (!streaming_.load(std::memory_order_acquire)) {
      reconnect_drop_.fetch_add(1, std::memory_order_relaxed);
    }
    return;
  }
  if (frame.lost_packets > 0) {
    sdk_net_drop_.fetch_add(1, std::memory_order_relaxed);
  }
  const bool soft = software_source_.load(std::memory_order_acquire);
  const bool trigger = trigger_mode_.load(std::memory_order_acquire);
  TriggerToken token;
  if (soft) {
    if (!tokens_.TryPop(&token)) {
      orphan_drop_.fetch_add(1, std::memory_order_relaxed);
      logger_.Error(config_.name, "孤儿迟到帧已丢弃，不绑定下一次触发");
      return;
    }
  }
  if (!trigger) {
    const uint64_t previous = last_frame_id_.load(std::memory_order_relaxed);
    if (!exempt_gap_.exchange(false) && previous != 0 && frame.frame_id > previous + 1) {
      hw_frame_gap_.fetch_add(frame.frame_id - previous - 1, std::memory_order_relaxed);
    }
  } else {
    exempt_gap_.store(false);
  }
  if (frame.frame_id != 0) {
    last_frame_id_.store(frame.frame_id, std::memory_order_relaxed);
  }

  ShmWriteRequest write;
  write.data = frame.data;
  write.size = frame.size;
  write.width = frame.width;
  write.height = frame.height;
  write.step = frame.step;
  write.pixel_format = frame.pixel_format;
  write.frame_id = frame.frame_id == 0 ? last_frame_id_.load() : frame.frame_id;
  if (write.frame_id == 0) {
    write.frame_id = 1;
  }
  write.param_version_seq = soft ? token.param_version_seq : version_.load(std::memory_order_acquire);
  write.trigger_seq_id = soft ? token.trigger_seq_id : 0;
  const ShmWriteResult result = shm_.TryWrite(write);
  if (result.status == ShmWriteStatus::kFullDrop) {
    shm_full_drop_.fetch_add(1, std::memory_order_relaxed);
    logger_.Warn(config_.name, "共享内存写满，本帧非阻塞丢弃");
    return;
  }
  if (result.status == ShmWriteStatus::kTooLarge || result.status == ShmWriteStatus::kWrongThread ||
      result.status == ShmWriteStatus::kNotReady) {
    logger_.Error(config_.name, "共享内存写入被拒绝");
    return;
  }
  if (result.status == ShmWriteStatus::kForceReclaim) {
    shm_force_reclaim_.fetch_add(1, std::memory_order_relaxed);
  }
  trigger_ok_.fetch_add(1, std::memory_order_relaxed);
  write_ns_sum_.fetch_add(result.write_ns, std::memory_order_relaxed);
  write_count_.fetch_add(1, std::memory_order_relaxed);
  uint32_t peak = active_peak_.load();
  while (result.active_slots > peak &&
         !active_peak_.compare_exchange_weak(peak, result.active_slots)) {
  }

  FrameNotice notice;
  notice.stamp_system_ns = SystemNs();
  notice.optical_frame_id = config_.optical_frame_id;
  notice.camera_id = config_.name;
  notice.capture_mode = trigger ? 1 : 0;
  notice.trigger_seq_id = write.trigger_seq_id;
  notice.workpiece_id = soft ? token.workpiece_id : "";
  notice.param_version_seq = write.param_version_seq;
  notice.shm_name = config_.shm_name;
  notice.slot_index = result.slot_index;
  notice.data_offset = result.data_offset;
  notice.data_size = frame.size;
  notice.frame_id = write.frame_id;
  notice.width = frame.width;
  notice.height = frame.height;
  notice.step = frame.step;
  notice.pixel_format = frame.pixel_format;
  notice.exposure_time_us = UnpackFloat(exposure_bits_.load());
  notice.gain_db = UnpackFloat(gain_bits_.load());
  PushNotice(std::move(notice));
}

void CameraStationWorker::PushNotice(FrameNotice notice) {
  {
    std::lock_guard<std::mutex> lock(notice_mutex_);
    notices_.push_back(std::move(notice));
    notice_pending_.store(true);
  }
  cmd_cv_.notify_all();
}

void CameraStationWorker::PublishPending() {
  std::vector<FrameNotice> local;
  {
    std::lock_guard<std::mutex> lock(notice_mutex_);
    local.swap(notices_);
    notice_pending_.store(false);
  }
  Notifier notifier;
  {
    std::lock_guard<std::mutex> lock(notifier_mutex_);
    notifier = notifier_;
  }
  if (!notifier) {
    return;
  }
  for (const auto& notice : local) {
    notifier(notice);
  }
}

void CameraStationWorker::Loop() {
  while (!stop_.load()) {
    {
      std::unique_lock<std::mutex> lock(cmd_mutex_);
      cmd_cv_.wait_for(lock, std::chrono::milliseconds(30), [&] {
        return stop_.load() || notice_pending_.load() || tune_pending_.load() ||
               reconnect_requested_.load() || !pending_triggers_.empty();
      });
    }
    PublishPending();
    if (stop_.load()) {
      break;
    }
    if (reconnect_requested_.load()) {
      HandleReconnect();
    }
    if (tune_pending_.exchange(false)) {
      if (auto task = tuner_.Take()) {
        ApplyTune(*task);
      }
    }
    std::vector<TriggerRequest> triggers;
    {
      std::lock_guard<std::mutex> lock(cmd_mutex_);
      triggers.swap(pending_triggers_);
    }
    for (const auto& trigger : triggers) {
      ApplyTrigger(trigger);
    }
    const int expired = tokens_.ExpireOlderThan(
        SteadyNs(), static_cast<uint64_t>(config_.trigger_timeout_ms) * 1000000ULL);
    if (expired > 0) {
      trigger_timeout_drop_.fetch_add(static_cast<uint64_t>(expired), std::memory_order_relaxed);
      logger_.Error(config_.name, "触发超时未出图，已丢弃过期令牌");
    }
    MaybeStat();
  }
}

uint64_t CameraStationWorker::PeriodOf(std::atomic<uint64_t>* value, std::atomic<uint64_t>* mark) {
  const uint64_t now = value->load();
  const uint64_t previous = mark->exchange(now);
  return now >= previous ? now - previous : now;
}

std::string CameraStationWorker::FormatStat() {
  const uint64_t frames = PeriodOf(&trigger_ok_, &mark_trigger_);
  const uint64_t tunes = PeriodOf(&tune_apply_, &mark_tune_);
  const uint64_t skips = PeriodOf(&tune_skip_, &mark_skip_);
  const uint64_t write_ns = PeriodOf(&write_ns_sum_, &mark_write_ns_);
  const uint64_t writes = PeriodOf(&write_count_, &mark_write_count_);
  const double avg_ms = writes == 0 ? 0.0 : static_cast<double>(write_ns) / static_cast<double>(writes) / 1e6;
  const uint32_t peak = active_peak_.exchange(0);
  std::ostringstream out;
  out << std::fixed;
  const std::string mode = !trigger_mode_.load()
                               ? "CONTINUOUS"
                               : (software_source_.load() ? "TRIGGER(Software)"
                                                          : "TRIGGER(" + config_.trigger_source + ")");
  out << "模式: " << mode << " | 周期: " << config_.stat_report_interval_sec << ".0s"
      << " | 触发成功: " << frames << " 帧"
      << " | 算法闭环调参: " << tunes << " 次 (防抖跳过: " << skips << " 次)"
      << " | 当前参数(Ver=" << version_.load() << "): Exp=" << std::setprecision(1)
      << UnpackFloat(exposure_bits_.load()) << "us, Gain=" << std::setprecision(1)
      << UnpackFloat(gain_bits_.load()) << "dB"
      << " | 平均SHM写耗时: " << std::setprecision(2) << avg_ms << "ms"
      << " | 活跃槽位峰值: " << peak << "/" << config_.shm_slot_count
      << " | 异常统计 -> [SDK网口丢包: " << PeriodOf(&sdk_net_drop_, &mark_sdk_)
      << " | 硬件帧断层: " << PeriodOf(&hw_frame_gap_, &mark_gap_)
      << " | SHM写满丢帧: " << PeriodOf(&shm_full_drop_, &mark_full_)
      << " | SHM超时强占: " << PeriodOf(&shm_force_reclaim_, &mark_reclaim_)
      << " | 触发超时未出图: " << PeriodOf(&trigger_timeout_drop_, &mark_timeout_)
      << " | 断线重连丢帧: " << PeriodOf(&reconnect_drop_, &mark_reconnect_)
      << " | 孤儿迟到帧: " << PeriodOf(&orphan_drop_, &mark_orphan_) << "]";
  return out.str();
}

void CameraStationWorker::MaybeStat() {
  const int64_t now = static_cast<int64_t>(SteadyNs());
  int64_t previous = last_stat_ns_.load();
  const int64_t interval = static_cast<int64_t>(config_.stat_report_interval_sec) * 1000000000LL;
  if (now - previous < interval) {
    return;
  }
  if (!last_stat_ns_.compare_exchange_strong(previous, now)) {
    return;
  }
  logger_.Stat(config_.name, FormatStat());
}

}  // namespace industrial_camera
