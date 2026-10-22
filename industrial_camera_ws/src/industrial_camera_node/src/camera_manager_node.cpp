#include "node/camera_manager_node.hpp"

#include "common/config_validator.hpp"
#include "common/yaml_config.hpp"
#include "hal/hik_camera_plugin.hpp"

#include <iostream>
#include <sstream>

namespace industrial_camera {

std::unique_ptr<ICameraDevice> CreateHikDevice(const StationConfig&) {
  return std::make_unique<HikCameraPlugin>();
}

CameraRuntime::CameraRuntime(CameraLogger& logger) : logger_(logger) {}

CameraRuntime::~CameraRuntime() { Stop(); }

bool CameraRuntime::Start(const std::string& topology_path, Factory factory, std::string* error) {
  Stop();
  LoadResult loaded = LoadProjectConfig(topology_path);
  if (!loaded.ok) {
    if (error != nullptr) {
      *error = loaded.error;
    }
    return false;
  }
  const ValidationResult valid = ValidateProject(loaded.config);
  if (!valid.ok) {
    std::ostringstream joined;
    for (const auto& item : valid.errors) {
      joined << item << "; ";
    }
    if (error != nullptr) {
      *error = joined.str();
    }
    return false;
  }
  std::string log_error;
  if (!logger_.Open(loaded.config.log_dir, &log_error)) {
    if (error != nullptr) {
      *error = log_error;
    }
    return false;
  }
  logger_owned_ = true;
  config_ = std::move(loaded.config);
  for (const auto& station : config_.stations) {
    auto device = factory(station);
    if (device == nullptr) {
      Stop();
      if (error != nullptr) {
        *error = station.name + " 设备工厂返回空指针";
      }
      return false;
    }
    auto worker = std::make_unique<CameraStationWorker>(station, std::move(device), logger_);
    if (notifier_) {
      worker->SetNotifier(notifier_);
    }
    std::string start_error;
    if (!worker->Start(&start_error)) {
      Stop();
      if (error != nullptr) {
        *error = station.name + " 启动失败: " + start_error;
      }
      return false;
    }
    workers_.push_back(std::move(worker));
  }
  std::cout << "trigger_cmd_topic=" << config_.trigger_cmd_topic << '\n'
            << "algo_tune_topic=" << config_.algo_tune_topic << '\n'
            << "set_param_service=" << config_.set_param_service << '\n';
  for (const auto& station : config_.stations) {
    std::cout << "station=" << station.name << " topic=" << station.topic_name
              << " shm=" << station.shm_name << " mode=" << station.working_mode << '\n';
  }
  return true;
}

void CameraRuntime::Stop() {
  workers_.clear();
  if (logger_owned_) {
    logger_.Close();
    logger_owned_ = false;
  }
}

bool CameraRuntime::SubmitTrigger(const TriggerRequest& request, std::string* error) {
  auto* worker = Find(request.camera_id);
  if (worker == nullptr) {
    if (error != nullptr) {
      *error = "未知相机: " + request.camera_id;
    }
    return false;
  }
  return worker->SubmitTrigger(request, error);
}

bool CameraRuntime::SubmitTune(const TuneRequest& request, std::string* error) {
  auto* worker = Find(request.camera_id);
  if (worker == nullptr) {
    if (error != nullptr) {
      *error = "未知相机: " + request.camera_id;
    }
    return false;
  }
  return worker->SubmitTune(request, error);
}

ParamWriteResult CameraRuntime::SetParam(const ParamWriteRequest& request) {
  auto* worker = Find(request.camera_id);
  if (worker == nullptr) {
    ParamWriteResult result;
    result.message = "未知相机: " + request.camera_id;
    return result;
  }
  return worker->SetParam(request);
}

void CameraRuntime::SetNotifier(Notifier notifier) {
  notifier_ = std::move(notifier);
  for (auto& worker : workers_) {
    worker->SetNotifier(notifier_);
  }
}

CameraStationWorker* CameraRuntime::Find(const std::string& camera_id) {
  for (auto& worker : workers_) {
    if (worker->Config().name == camera_id) {
      return worker.get();
    }
  }
  return nullptr;
}

}  // namespace industrial_camera
