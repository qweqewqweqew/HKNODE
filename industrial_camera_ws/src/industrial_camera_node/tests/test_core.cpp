#include "common/camera_logger.hpp"
#include "common/config_validator.hpp"
#include "common/types.hpp"
#include "common/yaml_config.hpp"
#include "hal/i_camera_device.hpp"
#include "node/auto_tune_controller.hpp"
#include "node/camera_station_worker.hpp"
#include "node/trigger_token_queue.hpp"
#include "shm/shm_image_client.hpp"
#include "shm/shm_layout.hpp"
#include "shm/shm_ring_buffer.hpp"

#include <chrono>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace {

int g_failed = 0;

void Check(bool condition, const char* text, int line) {
  if (!condition) {
    std::cerr << "FAIL " << line << ": " << text << '\n';
    ++g_failed;
  }
}

#define EXPECT_TRUE(cond) Check(static_cast<bool>(cond), #cond, __LINE__)
#define EXPECT_EQ(a, b) Check((a) == (b), #a " == " #b, __LINE__)

bool WaitFor(const std::function<bool()>& predicate, int timeout_ms) {
  const auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout_ms);
  while (std::chrono::steady_clock::now() < deadline) {
    if (predicate()) {
      return true;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  return predicate();
}

class FakeCameraDevice : public industrial_camera::ICameraDevice {
 public:
  uint64_t payload{64};
  bool auto_emit{true};
  int open_count{0};
  int violations{0};
  int last_code{0};
  std::vector<std::string> ops;
  std::map<std::string, std::string> values;
  std::vector<uint8_t> pattern{8, 1, 2, 3, 4, 5, 6, 7};

  bool Open(const std::string&, const std::string&, std::string*) override {
    ++open_count;
    open_ = true;
    ops.push_back("Open");
    return true;
  }
  void Close() override {
    if (open_) {
      ops.push_back("Close");
    }
    open_ = false;
    grabbing_ = false;
  }
  bool IsOpen() const override { return open_; }
  bool RegisterImageCallback(industrial_camera::ImageCallback callback, std::string*) override {
    image_ = std::move(callback);
    ops.push_back("RegisterImageCallBack");
    return true;
  }
  bool RegisterExceptionCallback(industrial_camera::ExceptionCallback callback,
                                 std::string*) override {
    exception_ = std::move(callback);
    ops.push_back("RegisterExceptionCallBack");
    return true;
  }
  bool StartGrabbing(std::string*) override {
    grabbing_ = true;
    ops.push_back("StartGrabbing");
    return true;
  }
  bool StopGrabbing(std::string*) override {
    if (grabbing_) {
      ops.push_back("StopGrabbing");
    }
    grabbing_ = false;
    return true;
  }
  bool ApplyFeature(const industrial_camera::FeatureEntry& feature, std::string* error) override {
    switch (feature.type) {
      case industrial_camera::ParamType::kInt:
        return SetInt(feature.feature, std::stoll(feature.value), error);
      case industrial_camera::ParamType::kFloat:
        return SetFloat(feature.feature, std::stod(feature.value), error);
      case industrial_camera::ParamType::kEnum:
        return SetEnum(feature.feature, feature.value, error);
      case industrial_camera::ParamType::kBool:
        return SetBool(feature.feature, feature.value == "true" || feature.value == "1", error);
      case industrial_camera::ParamType::kString:
        return SetString(feature.feature, feature.value, error);
      case industrial_camera::ParamType::kCmd:
        return SetCommand(feature.feature, error);
    }
    return false;
  }
  bool SetInt(const std::string& key, int64_t value, std::string*) override {
    Note();
    values[key] = std::to_string(value);
    ops.push_back("SetInt:" + key);
    return true;
  }
  bool SetFloat(const std::string& key, double value, std::string*) override {
    Note();
    values[key] = std::to_string(value);
    ops.push_back("SetFloat:" + key + "=" + values[key]);
    return true;
  }
  bool SetEnum(const std::string& key, const std::string& value, std::string*) override {
    Note();
    values[key] = value;
    ops.push_back("SetEnum:" + key + "=" + value);
    return true;
  }
  bool SetBool(const std::string& key, bool value, std::string*) override {
    Note();
    ops.push_back(std::string("SetBool:") + key);
    values[key] = value ? "true" : "false";
    return true;
  }
  bool SetString(const std::string& key, const std::string& value, std::string*) override {
    Note();
    ops.push_back("SetString:" + key);
    values[key] = value;
    return true;
  }
  bool SetCommand(const std::string& key, std::string*) override {
    Note();
    ops.push_back("SetCommand:" + key);
    if (key == "TriggerSoftware" && auto_emit && image_ && grabbing_) {
      EmitFrame(0, 0);
    }
    return true;
  }
  bool GetInt(const std::string& key, int64_t* value, std::string*) override {
    Note();
    ops.push_back("GetInt:" + key);
    if (value != nullptr) {
      *value = static_cast<int64_t>(payload);
    }
    return true;
  }
  bool GetPayloadSize(uint64_t* bytes, std::string* error) override {
    int64_t value = 0;
    if (!GetInt("PayloadSize", &value, error)) {
      return false;
    }
    if (bytes != nullptr) {
      *bytes = static_cast<uint64_t>(value);
    }
    return true;
  }
  int LastSdkCode() const override { return last_code; }

  void EmitFrame(uint32_t lost, uint64_t frame_id) {
    if (!image_) {
      return;
    }
    industrial_camera::RawFrame frame;
    frame.data = pattern.data();
    frame.size = static_cast<uint32_t>(pattern.size());
    frame.width = 8;
    frame.height = 1;
    frame.step = 8;
    frame.pixel_format = 0;
    frame.frame_id = frame_id == 0 ? next_frame_++ : frame_id;
    if (frame_id == 0) {
      // next_frame_ already incremented
    } else {
      next_frame_ = frame_id + 1;
    }
    frame.lost_packets = lost;
    const bool previous = in_callback_;
    in_callback_ = true;
    image_(frame);
    in_callback_ = previous;
  }

  void EmitDisconnect() {
    if (exception_) {
      exception_(0x80000206);
    }
  }

 private:
  void Note() {
    if (in_callback_) {
      ++violations;
    }
  }

  bool open_{false};
  bool grabbing_{false};
  bool in_callback_{false};
  uint64_t next_frame_{1};
  industrial_camera::ImageCallback image_;
  industrial_camera::ExceptionCallback exception_;
};

industrial_camera::StationConfig MakeStation(const std::string& name, const std::string& mode) {
  industrial_camera::StationConfig station;
  station.name = name;
  station.brand = "hikvision";
  station.connect_type = "sn";
  station.device_identity = name + "_sn";
  station.optical_frame_id = name + "_optical";
  station.working_mode = mode;
  station.trigger_source = "software";
  station.trigger_timeout_ms = 200;
  station.topic_name = "/vision/" + name;
  station.shm_name = "ut_" + name;
  station.shm_slot_count = 2;
  station.shm_slot_capacity_mb = 1;
  station.shm_lock_timeout_ms = 50;
  station.reconnect_interval_ms = 100;
  station.param_settle_us = 0;
  station.trigger_queue_depth = 1;
  station.auto_tune_enable = true;
  station.auto_tune_exposure_min_us = 100;
  station.auto_tune_exposure_max_us = 20000;
  station.auto_tune_gain_min_db = 0;
  station.auto_tune_gain_max_db = 15;
  station.stat_report_interval_sec = 30;
  industrial_camera::FeatureEntry exposure;
  exposure.feature = "ExposureTime";
  exposure.type = industrial_camera::ParamType::kFloat;
  exposure.value = "2000";
  industrial_camera::FeatureEntry gain;
  gain.feature = "Gain";
  gain.type = industrial_camera::ParamType::kFloat;
  gain.value = "6";
  station.features.push_back(exposure);
  station.features.push_back(gain);
  return station;
}

bool HasOp(const std::vector<std::string>& ops, const std::string& text) {
  for (const auto& op : ops) {
    if (op.find(text) != std::string::npos) {
      return true;
    }
  }
  return false;
}

int IndexOf(const std::vector<std::string>& ops, const std::string& text) {
  for (int i = 0; i < static_cast<int>(ops.size()); ++i) {
    if (ops[i].find(text) != std::string::npos) {
      return i;
    }
  }
  return -1;
}

void TestLayout() {
  EXPECT_EQ(sizeof(industrial_camera::ShmSlotHeader), 64u);
  EXPECT_EQ(sizeof(industrial_camera::ShmControlBlock), 64u);
  EXPECT_EQ(industrial_camera::kShmMagic, 0x48494B53u);
  EXPECT_EQ(industrial_camera::kShmProtocolVersion, 4u);
}

void TestTokenAndTune() {
  industrial_camera::TriggerTokenQueue queue(1);
  industrial_camera::TriggerToken token;
  token.trigger_seq_id = 7;
  token.enqueue_time_ns = 1000;
  EXPECT_TRUE(queue.TryPush(token));
  EXPECT_TRUE(!queue.TryPush(token));
  EXPECT_EQ(queue.ExpireOlderThan(1000 + 500000000ULL, 200000000ULL), 1);
  EXPECT_EQ(queue.Size(), 0u);

  std::atomic<uint32_t> version{5};
  industrial_camera::AutoTuneController tuner(version);
  industrial_camera::TuneRequest fresh;
  fresh.base_param_version = 5;
  fresh.target_exposure_us = 1000;
  industrial_camera::TuneRequest stale;
  stale.base_param_version = 4;
  EXPECT_TRUE(tuner.Accept(fresh));
  EXPECT_TRUE(!tuner.Accept(stale));
  auto taken = tuner.Take();
  EXPECT_TRUE(taken.has_value());
  EXPECT_EQ(taken->base_param_version, 5u);
  EXPECT_EQ(taken->target_exposure_us, 1000.F);
}

void TestConfig() {
  const std::string root = TEST_SOURCE_DIR;
  auto loaded = industrial_camera::LoadProjectConfig(root + "/config/project_topology.yaml");
  EXPECT_TRUE(loaded.ok);
  if (!loaded.ok) {
    std::cerr << loaded.error << '\n';
    return;
  }
  EXPECT_EQ(loaded.config.stations.size(), 2u);
  EXPECT_EQ(loaded.config.stations[0].name, std::string("station1_up"));
  EXPECT_EQ(loaded.config.stations[0].param_settle_us, 0);
  EXPECT_EQ(loaded.config.stations[0].trigger_queue_depth, 1);
  EXPECT_TRUE(!loaded.config.stations[0].features.empty());
  auto valid = industrial_camera::ValidateProject(loaded.config);
  EXPECT_TRUE(valid.ok);

  loaded.config.stations[1].shm_name = loaded.config.stations[0].shm_name;
  valid = industrial_camera::ValidateProject(loaded.config);
  EXPECT_TRUE(!valid.ok);

  auto bad = industrial_camera::LoadProjectConfig(root + "/tests/fixtures/bad_topology.yaml");
  EXPECT_TRUE(!bad.ok);
  EXPECT_TRUE(bad.error.find("白名单") != std::string::npos ||
              bad.error.find("type") != std::string::npos);
}

void TestShm() {
  industrial_camera::ShmRingBuffer buffer;
  std::string error;
  EXPECT_TRUE(buffer.Create("ut_shm_basic", 2, 1024 * 1024, 60000ULL * 1000000ULL, &error));
  std::vector<uint8_t> bytes{9, 8, 7, 6};
  industrial_camera::ShmWriteRequest request;
  request.data = bytes.data();
  request.size = static_cast<uint32_t>(bytes.size());
  request.width = 2;
  request.height = 2;
  request.step = 2;
  request.frame_id = 11;
  request.param_version_seq = 3;
  request.trigger_seq_id = 42;
  const auto wrote = buffer.TryWrite(request);
  EXPECT_EQ(static_cast<int>(wrote.status), static_cast<int>(industrial_camera::ShmWriteStatus::kOk));

  industrial_camera::ShmImageClient client;
  EXPECT_TRUE(client.Open("ut_shm_basic", &error));
  auto guard = client.Lock(wrote.slot_index, 11, 42, &error);
  EXPECT_TRUE(guard.valid());
  EXPECT_TRUE(guard.StillValid());
  EXPECT_EQ(std::memcmp(guard.data(), bytes.data(), bytes.size()), 0);

  request.frame_id = 12;
  request.trigger_seq_id = 43;
  const auto second = buffer.TryWrite(request);
  EXPECT_EQ(static_cast<int>(second.status), static_cast<int>(industrial_camera::ShmWriteStatus::kOk));
  auto guard2 = client.Lock(second.slot_index, 12, 43, &error);
  EXPECT_TRUE(guard2.valid());
  const auto full = buffer.TryWrite(request);
  EXPECT_EQ(static_cast<int>(full.status),
            static_cast<int>(industrial_camera::ShmWriteStatus::kFullDrop));
  EXPECT_TRUE(guard.StillValid());
  guard = {};
  guard2 = {};

  std::thread other([&buffer, &request] {
    request.frame_id = 13;
    const auto denied = buffer.TryWrite(request);
    EXPECT_EQ(static_cast<int>(denied.status),
              static_cast<int>(industrial_camera::ShmWriteStatus::kWrongThread));
  });
  other.join();

  buffer.Reset();
  industrial_camera::ShmRingBuffer again;
  EXPECT_TRUE(again.Create("ut_shm_basic", 2, 1024 * 1024, 50ULL * 1000000ULL, &error));
  industrial_camera::ShmImageClient restarted;
  EXPECT_TRUE(restarted.Open("ut_shm_basic", &error));
  EXPECT_EQ(restarted.Control()->magic_num, industrial_camera::kShmMagic);
  EXPECT_EQ(restarted.Control()->version, 4u);
}

void TestForceReclaim() {
  industrial_camera::ShmRingBuffer buffer;
  std::string error;
  EXPECT_TRUE(buffer.Create("ut_shm_reclaim", 2, 1024 * 1024, 50ULL * 1000000ULL, &error));
  std::vector<uint8_t> bytes{1, 2, 3, 4};
  industrial_camera::ShmWriteRequest request;
  request.data = bytes.data();
  request.size = 4;
  request.frame_id = 1;
  request.trigger_seq_id = 1;
  const auto first = buffer.TryWrite(request);
  request.frame_id = 2;
  request.trigger_seq_id = 2;
  const auto second = buffer.TryWrite(request);
  industrial_camera::ShmImageClient client;
  EXPECT_TRUE(client.Open("ut_shm_reclaim", &error));
  auto hold_a = client.Lock(first.slot_index, 1, 1, &error);
  auto hold_b = client.Lock(second.slot_index, 2, 2, &error);
  EXPECT_TRUE(hold_a.valid());
  EXPECT_TRUE(hold_b.valid());
  std::this_thread::sleep_for(std::chrono::milliseconds(120));
  request.frame_id = 3;
  request.trigger_seq_id = 3;
  const auto reclaimed = buffer.TryWrite(request);
  EXPECT_EQ(static_cast<int>(reclaimed.status),
            static_cast<int>(industrial_camera::ShmWriteStatus::kForceReclaim));
  EXPECT_TRUE(!hold_a.StillValid() || !hold_b.StillValid());
}

void TestWorker() {
  industrial_camera::CameraLogger logger;
  std::string error;
  EXPECT_TRUE(logger.Open("./logs/camera_core_tests", &error));
  std::mutex log_mu;
  std::vector<std::string> lines;
  logger.SetSinkForTest([&](const std::string& line) {
    std::lock_guard<std::mutex> lock(log_mu);
    lines.push_back(line);
  });

  auto device = std::make_unique<FakeCameraDevice>();
  auto* fake = device.get();
  auto station = MakeStation("trig", "trigger");
  station.stat_report_interval_sec = 1;
  industrial_camera::CameraStationWorker worker(station, std::move(device), logger);
  std::mutex notice_mu;
  std::vector<industrial_camera::FrameNotice> notices;
  worker.SetNotifier([&](const industrial_camera::FrameNotice& notice) {
    std::lock_guard<std::mutex> lock(notice_mu);
    notices.push_back(notice);
  });
  EXPECT_TRUE(worker.Start(&error));
  if (!error.empty() && !worker.ShmBase()) {
    std::cerr << error << '\n';
  }
  const int register_at = IndexOf(fake->ops, "RegisterImageCallBack");
  const int start_at = IndexOf(fake->ops, "StartGrabbing");
  EXPECT_TRUE(register_at >= 0 && start_at > register_at);
  EXPECT_TRUE(IndexOf(fake->ops, "RegisterExceptionCallBack") < start_at);
  EXPECT_TRUE(HasOp(fake->ops, "SetEnum:AcquisitionMode=Continuous"));
  EXPECT_TRUE(HasOp(fake->ops, "SetEnum:TriggerMode=On"));
  EXPECT_TRUE(!HasOp(fake->ops, "SingleFrame"));

  industrial_camera::TriggerRequest trigger;
  trigger.camera_id = "trig";
  trigger.trigger_seq_id = 100;
  trigger.workpiece_id = "WP1";
  trigger.override_exposure_us = 3000;
  trigger.override_gain_db = -1;
  EXPECT_TRUE(worker.SubmitTrigger(trigger, &error));
  EXPECT_TRUE(WaitFor([&] {
    std::lock_guard<std::mutex> lock(notice_mu);
    return !notices.empty();
  }, 1500));
  {
    std::lock_guard<std::mutex> lock(notice_mu);
    EXPECT_TRUE(!notices.empty());
    if (!notices.empty()) {
      EXPECT_EQ(notices.back().trigger_seq_id, 100ULL);
      EXPECT_EQ(notices.back().workpiece_id, std::string("WP1"));
      EXPECT_TRUE(notices.back().exposure_time_us > 2999.F);
    }
  }
  EXPECT_EQ(fake->violations, 0);
  EXPECT_TRUE(HasOp(fake->ops, "SetEnum:ExposureAuto=Off"));
  EXPECT_TRUE(HasOp(fake->ops, "SetCommand:TriggerSoftware"));
  const int auto_at = IndexOf(fake->ops, "SetEnum:ExposureAuto=Off");
  const int cmd_at = IndexOf(fake->ops, "SetCommand:TriggerSoftware");
  EXPECT_TRUE(auto_at >= 0 && auto_at < cmd_at);

  industrial_camera::ParamWriteRequest mode;
  mode.camera_id = "trig";
  mode.key = "@WorkingMode";
  mode.type = industrial_camera::ParamType::kEnum;
  mode.value = "continuous";
  const auto switched = worker.SetParam(mode);
  EXPECT_TRUE(switched.success);
  const int stop_at = IndexOf(fake->ops, "StopGrabbing");
  const int off_at = IndexOf(fake->ops, "SetEnum:TriggerMode=Off");
  EXPECT_TRUE(stop_at >= 0 && off_at > stop_at);

  industrial_camera::TuneRequest stale;
  stale.base_param_version = 0;
  stale.tune_exposure = true;
  stale.target_exposure_us = 500;
  EXPECT_TRUE(!worker.SubmitTune(stale, &error));
  EXPECT_TRUE(worker.Health().tune_skip >= 1);

  worker.Stop();

  auto slow_device = std::make_unique<FakeCameraDevice>();
  slow_device->auto_emit = false;
  auto* slow = slow_device.get();
  auto slow_station = MakeStation("slow", "trigger");
  slow_station.stat_report_interval_sec = 30;
  industrial_camera::CameraStationWorker slow_worker(slow_station, std::move(slow_device), logger);
  EXPECT_TRUE(slow_worker.Start(&error));
  industrial_camera::TriggerRequest late;
  late.trigger_seq_id = 8;
  late.workpiece_id = "LATE";
  EXPECT_TRUE(slow_worker.SubmitTrigger(late, &error));
  EXPECT_TRUE(WaitFor([&] { return slow_worker.Health().trigger_timeout_drop >= 1; }, 1500));
  slow->EmitFrame(0, 50);
  EXPECT_TRUE(WaitFor([&] { return slow_worker.Health().orphan_drop >= 1; }, 1000));
  slow_worker.Stop();

  auto gap_device = std::make_unique<FakeCameraDevice>();
  gap_device->auto_emit = false;
  auto* gap = gap_device.get();
  auto gap_station = MakeStation("gap", "continuous");
  industrial_camera::CameraStationWorker gap_worker(gap_station, std::move(gap_device), logger);
  EXPECT_TRUE(gap_worker.Start(&error));
  gap->EmitFrame(1, 1);
  gap->EmitFrame(0, 5);
  EXPECT_TRUE(WaitFor([&] { return gap_worker.Health().hw_frame_gap >= 3; }, 1000));
  EXPECT_TRUE(gap_worker.Health().sdk_net_drop >= 1);
  const std::string stat = gap_worker.FormatStat();
  EXPECT_TRUE(stat.find("SDK网口丢包") != std::string::npos);
  EXPECT_TRUE(stat.find("孤儿迟到帧") != std::string::npos);
  gap_worker.Stop();

  auto big = std::make_unique<FakeCameraDevice>();
  big->payload = 2ULL * 1024ULL * 1024ULL;
  auto big_station = MakeStation("big", "trigger");
  industrial_camera::CameraStationWorker big_worker(big_station, std::move(big), logger);
  std::string payload_error;
  EXPECT_TRUE(!big_worker.Start(&payload_error));
  EXPECT_TRUE(payload_error.find("预检失败") != std::string::npos);

  auto link = std::make_unique<FakeCameraDevice>();
  link->auto_emit = false;
  auto* link_fake = link.get();
  auto link_station = MakeStation("link", "trigger");
  industrial_camera::CameraStationWorker link_worker(link_station, std::move(link), logger);
  EXPECT_TRUE(link_worker.Start(&error));
  uint8_t* base = link_worker.ShmBase();
  industrial_camera::TriggerRequest pending;
  pending.trigger_seq_id = 3;
  EXPECT_TRUE(link_worker.SubmitTrigger(pending, &error));
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  link_fake->EmitDisconnect();
  EXPECT_TRUE(WaitFor([&] { return link_fake->open_count >= 2; }, 2000));
  EXPECT_EQ(link_worker.ShmBase(), base);
  EXPECT_TRUE(link_worker.Health().reconnect_drop >= 1);
  link_worker.Stop();
}

}  // namespace

int main() {
  TestLayout();
  TestTokenAndTune();
  TestConfig();
  TestShm();
  TestForceReclaim();
  TestWorker();
  if (g_failed != 0) {
    std::cerr << g_failed << " assertion(s) failed\n";
    return 1;
  }
  std::cout << "camera_core_tests passed\n";
  return 0;
}
