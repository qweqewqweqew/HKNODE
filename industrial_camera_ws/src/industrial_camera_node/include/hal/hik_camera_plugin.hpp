#pragma once

#include <memory>
#include <string>

#include "hal/i_camera_device.hpp"

namespace industrial_camera {

struct HikPluginState;

class HikCameraPlugin : public ICameraDevice {
 public:
  HikCameraPlugin();
  ~HikCameraPlugin() override;

  bool Open(const std::string& connect_type, const std::string& identity,
            std::string* error) override;
  void Close() override;
  bool IsOpen() const override;
  bool RegisterImageCallback(ImageCallback callback, std::string* error) override;
  bool RegisterExceptionCallback(ExceptionCallback callback, std::string* error) override;
  bool StartGrabbing(std::string* error) override;
  bool StopGrabbing(std::string* error) override;
  bool ApplyFeature(const FeatureEntry& feature, std::string* error) override;
  bool SetInt(const std::string& key, int64_t value, std::string* error) override;
  bool SetFloat(const std::string& key, double value, std::string* error) override;
  bool SetEnum(const std::string& key, const std::string& value, std::string* error) override;
  bool SetBool(const std::string& key, bool value, std::string* error) override;
  bool SetString(const std::string& key, const std::string& value, std::string* error) override;
  bool SetCommand(const std::string& key, std::string* error) override;
  bool GetInt(const std::string& key, int64_t* value, std::string* error) override;
  bool GetPayloadSize(uint64_t* bytes, std::string* error) override;
  int LastSdkCode() const override;

 private:
  std::unique_ptr<HikPluginState> state_;
};

}  // namespace industrial_camera
