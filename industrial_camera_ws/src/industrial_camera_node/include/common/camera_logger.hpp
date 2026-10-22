#pragma once

#include <functional>
#include <string>

namespace industrial_camera {

class CameraLogger {
 public:
  CameraLogger() = default;
  ~CameraLogger();

  CameraLogger(const CameraLogger&) = delete;
  CameraLogger& operator=(const CameraLogger&) = delete;

  bool Open(const std::string& directory, std::string* error);
  void Close();
  void Log(const std::string& level, const std::string& station, const std::string& message);
  void Info(const std::string& station, const std::string& message);
  void Warn(const std::string& station, const std::string& message);
  void Error(const std::string& station, const std::string& message);
  void Stat(const std::string& station, const std::string& body);
  void Throttle(const std::string& key, const std::string& level, const std::string& station,
                const std::string& message);
  void SetSinkForTest(std::function<void(const std::string&)> sink);

 private:
  struct Impl;
  Impl* impl_{nullptr};
};

}  // namespace industrial_camera
