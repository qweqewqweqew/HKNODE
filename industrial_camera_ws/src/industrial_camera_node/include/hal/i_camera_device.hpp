#pragma once

#include <functional>
#include <string>

#include "common/types.hpp"

namespace industrial_camera {

using ImageCallback = std::function<void(const RawFrame&)>;
using ExceptionCallback = std::function<void(int code)>;

class ICameraDevice {
 public:
  virtual ~ICameraDevice() = default;

  virtual bool Open(const std::string& connect_type, const std::string& identity,
                    std::string* error) = 0;
  virtual void Close() = 0;
  virtual bool IsOpen() const = 0;
  virtual bool RegisterImageCallback(ImageCallback callback, std::string* error) = 0;
  virtual bool RegisterExceptionCallback(ExceptionCallback callback, std::string* error) = 0;
  virtual bool StartGrabbing(std::string* error) = 0;
  virtual bool StopGrabbing(std::string* error) = 0;
  virtual bool ApplyFeature(const FeatureEntry& feature, std::string* error) = 0;
  virtual bool SetInt(const std::string& key, int64_t value, std::string* error) = 0;
  virtual bool SetFloat(const std::string& key, double value, std::string* error) = 0;
  virtual bool SetEnum(const std::string& key, const std::string& value, std::string* error) = 0;
  virtual bool SetBool(const std::string& key, bool value, std::string* error) = 0;
  virtual bool SetString(const std::string& key, const std::string& value, std::string* error) = 0;
  virtual bool SetCommand(const std::string& key, std::string* error) = 0;
  virtual bool GetInt(const std::string& key, int64_t* value, std::string* error) = 0;
  virtual bool GetPayloadSize(uint64_t* bytes, std::string* error) = 0;
  virtual int LastSdkCode() const = 0;
};

}  // namespace industrial_camera
