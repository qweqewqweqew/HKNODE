#include "common/config_validator.hpp"

#include <unordered_map>
#include <unordered_set>

namespace industrial_camera {
namespace {

void Add(ValidationResult* result, const std::string& text) {
  result->ok = false;
  result->errors.push_back(text);
}

bool ValidSource(const std::string& source) {
  return source == "software" || source == "line0" || source == "line1" || source == "line2" ||
         source == "line3";
}

}  // namespace

ValidationResult ValidateProject(const ProjectConfig& config) {
  ValidationResult result;
  if (config.stations.empty()) {
    Add(&result, "没有可启动的工位");
    return result;
  }
  if (config.stat_report_interval_sec <= 0) {
    Add(&result, "stat_report_interval_sec 必须大于 0");
  }
  std::unordered_map<std::string, std::string> identities;
  std::unordered_map<std::string, std::string> shm_names;
  std::unordered_map<std::string, std::string> topics;
  std::unordered_set<std::string> station_names;
  for (const auto& station : config.stations) {
    if (!station_names.insert(station.name).second) {
      Add(&result, "工位名重复: " + station.name);
    }
    if (station.brand != "hikvision") {
      Add(&result, station.name + " 的 brand 必须是 hikvision");
    }
    if (station.connect_type != "sn" && station.connect_type != "ip") {
      Add(&result, station.name + " 的 connect_type 只能是 sn 或 ip");
    }
    if (station.device_identity.empty()) {
      Add(&result, station.name + " 缺少 device_identity");
    } else if (!identities.emplace(station.device_identity, station.name).second) {
      Add(&result, "device_identity 重复: " + station.device_identity);
    }
    if (station.shm_name.empty() || station.shm_name.find('\\') != std::string::npos ||
        station.shm_name.find('/') != std::string::npos) {
      Add(&result, station.name + " 的 shm_name 为空或包含斜杠");
    } else if (!shm_names.emplace(station.shm_name, station.name).second) {
      Add(&result, "shm_name 重复: " + station.shm_name);
    }
    if (station.topic_name.empty()) {
      Add(&result, station.name + " 缺少 topic_name");
    } else if (!topics.emplace(station.topic_name, station.name).second) {
      Add(&result, "topic_name 重复: " + station.topic_name);
    }
    if (station.shm_slot_count < 2 || station.shm_slot_count > 256) {
      Add(&result, station.name + " 的 shm_slot_count 必须在 2 到 256");
    }
    if (station.shm_slot_capacity_mb < 1) {
      Add(&result, station.name + " 的 shm_slot_capacity_mb 必须 >= 1");
    }
    if (station.shm_lock_timeout_ms < 50) {
      Add(&result, station.name + " 的 shm_lock_timeout_ms 必须 >= 50");
    }
    if (station.working_mode != "continuous" && station.working_mode != "trigger") {
      Add(&result, station.name + " 的 working_mode 只能是 continuous 或 trigger");
    }
    if (station.working_mode == "trigger" && !ValidSource(station.trigger_source)) {
      Add(&result, station.name + " 的 trigger_source 非法");
    }
    if (station.auto_tune_exposure_min_us >= station.auto_tune_exposure_max_us) {
      Add(&result, station.name + " 的曝光钳位下限必须小于上限");
    }
    if (station.auto_tune_gain_min_db >= station.auto_tune_gain_max_db) {
      Add(&result, station.name + " 的增益钳位下限必须小于上限");
    }
    if (station.param_settle_us < 0) {
      Add(&result, station.name + " 的 param_settle_us 不能为负");
    }
    if (station.trigger_queue_depth < 1) {
      Add(&result, station.name + " 的 trigger_queue_depth 必须 >= 1");
    }
    if (station.trigger_timeout_ms <= 0) {
      Add(&result, station.name + " 的 trigger_timeout_ms 必须大于 0");
    }
    if (station.reconnect_interval_ms < 100) {
      Add(&result, station.name + " 的 reconnect_interval_ms 必须 >= 100");
    }
    for (const auto& feature : station.features) {
      if (feature.feature.empty()) {
        Add(&result, station.name + " 存在空属性名");
      }
    }
  }
  return result;
}

}  // namespace industrial_camera
