#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace industrial_camera {

constexpr uint32_t kShmMagic = 0x48494B53;  // "HIKS"
constexpr uint32_t kShmProtocolVersion = 4;

enum class PixelFormat : uint32_t {
  kMono8 = 0,
  kBayerRG8 = 1,
  kBgr8 = 2,
  kRgb8 = 3,
  kYuv422 = 4,
  kUnknown = 255
};

enum class ParamType : uint8_t {
  kInt = 0,
  kFloat = 1,
  kEnum = 2,
  kBool = 3,
  kString = 4,
  kCmd = 5
};

struct FeatureEntry {
  std::string feature;
  ParamType type{ParamType::kEnum};
  std::string value;
};

struct StationConfig {
  std::string name;
  std::string brand{"hikvision"};
  std::string connect_type{"sn"};
  std::string device_identity;
  std::string optical_frame_id;
  std::string working_mode{"trigger"};
  std::string trigger_source{"software"};
  int trigger_timeout_ms{500};
  std::string topic_name;
  std::string shm_name;
  std::string shm_permissions{"0666"};
  int shm_slot_count{16};
  int shm_slot_capacity_mb{25};
  int shm_lock_timeout_ms{1500};
  int reconnect_interval_ms{3000};
  bool auto_tune_enable{true};
  double auto_tune_exposure_min_us{100.0};
  double auto_tune_exposure_max_us{20000.0};
  double auto_tune_gain_min_db{0.0};
  double auto_tune_gain_max_db{15.0};
  std::string feature_config_file;
  std::string resolved_feature_path;
  int param_settle_us{0};
  int trigger_queue_depth{1};
  int stat_report_interval_sec{10};
  std::vector<FeatureEntry> features;
};

struct ProjectConfig {
  std::string project_name;
  std::string log_dir{"./logs/camera_node"};
  std::string log_level{"INFO"};
  int stat_report_interval_sec{10};
  std::string trigger_cmd_topic{"/vision/trigger_cmd"};
  std::string algo_tune_topic{"/vision/algo_param_tune"};
  std::string set_param_service{"/vision/set_camera_param"};
  std::vector<StationConfig> stations;
};

struct RawFrame {
  const uint8_t* data{nullptr};
  uint32_t size{0};
  uint32_t width{0};
  uint32_t height{0};
  uint32_t step{0};
  uint32_t pixel_format{0};
  uint64_t frame_id{0};
  uint32_t lost_packets{0};
};

struct TriggerRequest {
  std::string camera_id;
  uint64_t trigger_seq_id{0};
  std::string workpiece_id;
  float override_exposure_us{0.F};
  float override_gain_db{-1.F};
};

struct TuneRequest {
  std::string camera_id;
  uint64_t base_frame_id{0};
  uint32_t base_param_version{0};
  bool tune_exposure{false};
  float target_exposure_us{0.F};
  bool tune_gain{false};
  float target_gain_db{0.F};
  bool retrigger_immediately{false};
  uint64_t retrigger_seq_id{0};
  std::string workpiece_id;
};

struct ParamWriteRequest {
  std::string camera_id;
  std::string key;
  ParamType type{ParamType::kFloat};
  std::string value;
};

struct ParamWriteResult {
  bool success{false};
  uint32_t new_param_version_seq{0};
  int sdk_error_code{0};
  std::string message;
};

struct FrameNotice {
  int64_t stamp_system_ns{0};
  std::string optical_frame_id;
  std::string camera_id;
  uint8_t capture_mode{0};
  uint64_t trigger_seq_id{0};
  std::string workpiece_id;
  uint32_t param_version_seq{0};
  std::string shm_name;
  uint32_t slot_index{0};
  uint64_t data_offset{0};
  uint32_t data_size{0};
  uint64_t frame_id{0};
  uint32_t width{0};
  uint32_t height{0};
  uint32_t step{0};
  uint32_t pixel_format{0};
  float exposure_time_us{0.F};
  float gain_db{0.F};
};

inline uint32_t PackFloat(float value) {
  static_assert(sizeof(float) == 4, "float must be 4 bytes");
  uint32_t bits = 0;
  std::memcpy(&bits, &value, sizeof(bits));
  return bits;
}

inline float UnpackFloat(uint32_t bits) {
  float value = 0.F;
  std::memcpy(&value, &bits, sizeof(value));
  return value;
}

inline const char* ParamTypeName(ParamType type) {
  switch (type) {
    case ParamType::kInt:
      return "int";
    case ParamType::kFloat:
      return "float";
    case ParamType::kEnum:
      return "enum";
    case ParamType::kBool:
      return "bool";
    case ParamType::kString:
      return "string";
    case ParamType::kCmd:
      return "cmd";
  }
  return "unknown";
}

inline bool ParseParamType(const std::string& text, ParamType* out) {
  if (text == "int") {
    *out = ParamType::kInt;
  } else if (text == "float") {
    *out = ParamType::kFloat;
  } else if (text == "enum") {
    *out = ParamType::kEnum;
  } else if (text == "bool") {
    *out = ParamType::kBool;
  } else if (text == "string") {
    *out = ParamType::kString;
  } else if (text == "cmd") {
    *out = ParamType::kCmd;
  } else {
    return false;
  }
  return true;
}

}  // namespace industrial_camera
