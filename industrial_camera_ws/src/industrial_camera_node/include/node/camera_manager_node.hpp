#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "common/camera_logger.hpp"
#include "common/types.hpp"
#include "node/camera_station_worker.hpp"

namespace industrial_camera {

std::unique_ptr<ICameraDevice> CreateHikDevice(const StationConfig& station);

class CameraRuntime {
 public:
  using Factory = std::function<std::unique_ptr<ICameraDevice>(const StationConfig&)>;
  using Notifier = CameraStationWorker::Notifier;

  explicit CameraRuntime(CameraLogger& logger);
  ~CameraRuntime();

  bool Start(const std::string& topology_path, Factory factory, std::string* error);
  void Stop();
  bool SubmitTrigger(const TriggerRequest& request, std::string* error);
  bool SubmitTune(const TuneRequest& request, std::string* error);
  ParamWriteResult SetParam(const ParamWriteRequest& request);
  void SetNotifier(Notifier notifier);

  const ProjectConfig& Config() const { return config_; }
  CameraStationWorker* Find(const std::string& camera_id);

 private:
  CameraLogger& logger_;
  ProjectConfig config_;
  Notifier notifier_;
  std::vector<std::unique_ptr<CameraStationWorker>> workers_;
  bool logger_owned_{false};
};

}  // namespace industrial_camera
