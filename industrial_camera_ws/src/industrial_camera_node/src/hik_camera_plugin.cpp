#include "hal/hik_camera_plugin.hpp"

#include "common/sdk_error.hpp"

#include <atomic>
#include <cstring>
#include <mutex>
#include <sstream>

#if defined(INDUSTRIAL_CAMERA_HAS_MVS)
#include "MvCameraControl.h"
#endif

namespace industrial_camera {
namespace {

std::atomic<int> g_sdk_users{0};

std::string FormatIp(unsigned int ip) {
  std::ostringstream out;
  out << ((ip >> 24) & 0xFF) << '.' << ((ip >> 16) & 0xFF) << '.' << ((ip >> 8) & 0xFF)
      << '.' << (ip & 0xFF);
  return out.str();
}

#if defined(INDUSTRIAL_CAMERA_HAS_MVS)
uint32_t MapPixel(unsigned int sdk_pixel) {
  switch (sdk_pixel) {
    case 0x01080001:
      return static_cast<uint32_t>(PixelFormat::kMono8);
    case 0x01080009:
      return static_cast<uint32_t>(PixelFormat::kBayerRG8);
    case 0x02180015:
      return static_cast<uint32_t>(PixelFormat::kBgr8);
    case 0x02180014:
      return static_cast<uint32_t>(PixelFormat::kRgb8);
    case 0x0210001F:
    case 0x02100032:
      return static_cast<uint32_t>(PixelFormat::kYuv422);
    default:
      return static_cast<uint32_t>(PixelFormat::kUnknown);
  }
}

void __stdcall OnSdkImage(unsigned char* data, MV_FRAME_OUT_INFO_EX* info, void* user);
void __stdcall OnSdkException(unsigned int msg_type, void* user);
#endif

}  // namespace

struct HikPluginState {
  std::mutex callback_mu;
  ImageCallback image_cb;
  ExceptionCallback exception_cb;
  int last_code{0};
  bool open{false};
  bool grabbing{false};
#if defined(INDUSTRIAL_CAMERA_HAS_MVS)
  void* handle{nullptr};
#endif

  bool Fail(int code, const std::string& action, std::string* error) {
    last_code = code;
    if (error != nullptr) {
      *error = action + ": " + TranslateSdkError(code);
    }
    return false;
  }
};

#if defined(INDUSTRIAL_CAMERA_HAS_MVS)
void __stdcall OnSdkImage(unsigned char* data, MV_FRAME_OUT_INFO_EX* info, void* user) {
  auto* impl = static_cast<HikPluginState*>(user);
  ImageCallback callback;
  {
    std::lock_guard<std::mutex> lock(impl->callback_mu);
    callback = impl->image_cb;
  }
  if (!callback || data == nullptr || info == nullptr) {
    return;
  }
  RawFrame frame;
  frame.data = data;
  frame.size = info->nFrameLen;
  frame.width = info->nWidth;
  frame.height = info->nHeight;
  frame.step = info->nHeight == 0 ? info->nFrameLen : info->nFrameLen / info->nHeight;
  frame.pixel_format = MapPixel(static_cast<unsigned int>(info->enPixelType));
  frame.frame_id = info->nFrameNum;
  frame.lost_packets = info->nLostPacket;
  callback(frame);
}

void __stdcall OnSdkException(unsigned int msg_type, void* user) {
  auto* impl = static_cast<HikPluginState*>(user);
  ExceptionCallback callback;
  {
    std::lock_guard<std::mutex> lock(impl->callback_mu);
    callback = impl->exception_cb;
  }
  if (callback) {
    callback(static_cast<int>(msg_type));
  }
}
#endif

HikCameraPlugin::HikCameraPlugin() : state_(std::make_unique<HikPluginState>()) {
#if defined(INDUSTRIAL_CAMERA_HAS_MVS)
  if (g_sdk_users.fetch_add(1) == 0) {
    MV_CC_Initialize();
  }
#else
  (void)g_sdk_users;
#endif
}

HikCameraPlugin::~HikCameraPlugin() {
  Close();
#if defined(INDUSTRIAL_CAMERA_HAS_MVS)
  if (g_sdk_users.fetch_sub(1) == 1) {
    MV_CC_Finalize();
  }
#endif
}

bool HikCameraPlugin::Open(const std::string& connect_type, const std::string& identity,
                           std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)connect_type;
  (void)identity;
  if (error != nullptr) {
    *error = "本构建未链接海康 MVS SDK。请安装 MVS，设置 MVS_DEV_DIR 后重新编译。";
  }
  return false;
#else
  Close();
  MV_CC_DEVICE_INFO_LIST list;
  std::memset(&list, 0, sizeof(list));
  int ret = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &list);
  if (ret != MV_OK) {
    return state_->Fail(ret, "枚举相机失败", error);
  }
  MV_CC_DEVICE_INFO* found = nullptr;
  for (unsigned int i = 0; i < list.nDeviceNum; ++i) {
    auto* info = list.pDeviceInfo[i];
    if (info == nullptr) {
      continue;
    }
    if (connect_type == "sn") {
      const char* serial = nullptr;
      if (info->nTLayerType == MV_GIGE_DEVICE) {
        serial = reinterpret_cast<const char*>(info->SpecialInfo.stGigEInfo.chSerialNumber);
      } else if (info->nTLayerType == MV_USB_DEVICE) {
        serial = reinterpret_cast<const char*>(info->SpecialInfo.stUsb3VInfo.chSerialNumber);
      }
      if (serial != nullptr && identity == serial) {
        found = info;
        break;
      }
    } else if (connect_type == "ip" && info->nTLayerType == MV_GIGE_DEVICE) {
      if (FormatIp(info->SpecialInfo.stGigEInfo.nCurrentIp) == identity) {
        found = info;
        break;
      }
    }
  }
  if (found == nullptr) {
    if (error != nullptr) {
      *error = "未找到相机: " + connect_type + "=" + identity;
    }
    return false;
  }
  ret = MV_CC_CreateHandle(&state_->handle, found);
  if (ret != MV_OK) {
    return state_->Fail(ret, "创建相机句柄失败", error);
  }
  ret = MV_CC_OpenDevice(state_->handle);
  if (ret != MV_OK) {
    MV_CC_DestroyHandle(state_->handle);
    state_->handle = nullptr;
    return state_->Fail(ret, "打开相机失败", error);
  }
  state_->open = true;
  state_->last_code = MV_OK;
  return true;
#endif
}

void HikCameraPlugin::Close() {
#if defined(INDUSTRIAL_CAMERA_HAS_MVS)
  if (state_->grabbing && state_->handle != nullptr) {
    MV_CC_StopGrabbing(state_->handle);
    state_->grabbing = false;
  }
  if (state_->open && state_->handle != nullptr) {
    MV_CC_CloseDevice(state_->handle);
    state_->open = false;
  }
  if (state_->handle != nullptr) {
    MV_CC_DestroyHandle(state_->handle);
    state_->handle = nullptr;
  }
#else
  state_->open = false;
  state_->grabbing = false;
#endif
}

bool HikCameraPlugin::IsOpen() const { return state_->open; }

bool HikCameraPlugin::RegisterImageCallback(ImageCallback callback, std::string* error) {
  {
    std::lock_guard<std::mutex> lock(state_->callback_mu);
    state_->image_cb = std::move(callback);
  }
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK，无法注册图像回调";
  }
  return false;
#else
  if (state_->handle == nullptr) {
    if (error != nullptr) {
      *error = "注册图像回调时句柄为空";
    }
    return false;
  }
  const int ret = MV_CC_RegisterImageCallBackEx(state_->handle, OnSdkImage, state_.get());
  if (ret != MV_OK) {
    return state_->Fail(ret, "注册图像回调失败", error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::RegisterExceptionCallback(ExceptionCallback callback, std::string* error) {
  {
    std::lock_guard<std::mutex> lock(state_->callback_mu);
    state_->exception_cb = std::move(callback);
  }
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK，无法注册异常回调";
  }
  return false;
#else
  if (state_->handle == nullptr) {
    if (error != nullptr) {
      *error = "注册异常回调时句柄为空";
    }
    return false;
  }
  const int ret = MV_CC_RegisterExceptionCallBack(state_->handle, OnSdkException, state_.get());
  if (ret != MV_OK) {
    return state_->Fail(ret, "注册异常回调失败", error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::StartGrabbing(std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK，无法开始取流";
  }
  return false;
#else
  const int ret = MV_CC_StartGrabbing(state_->handle);
  if (ret != MV_OK) {
    return state_->Fail(ret, "StartGrabbing 失败", error);
  }
  state_->grabbing = true;
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::StopGrabbing(std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)error;
  state_->grabbing = false;
  return true;
#else
  if (state_->handle == nullptr) {
    return true;
  }
  const int ret = MV_CC_StopGrabbing(state_->handle);
  state_->grabbing = false;
  if (ret != MV_OK) {
    return state_->Fail(ret, "StopGrabbing 失败", error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::ApplyFeature(const FeatureEntry& feature, std::string* error) {
  switch (feature.type) {
    case ParamType::kInt:
      return SetInt(feature.feature, std::stoll(feature.value), error);
    case ParamType::kFloat:
      return SetFloat(feature.feature, std::stod(feature.value), error);
    case ParamType::kEnum:
      return SetEnum(feature.feature, feature.value, error);
    case ParamType::kBool:
      return SetBool(feature.feature, feature.value == "1" || feature.value == "true" ||
                                         feature.value == "True",
                     error);
    case ParamType::kString:
      return SetString(feature.feature, feature.value, error);
    case ParamType::kCmd:
      return SetCommand(feature.feature, error);
  }
  if (error != nullptr) {
    *error = "不支持的属性类型";
  }
  return false;
}

bool HikCameraPlugin::SetInt(const std::string& key, int64_t value, std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)key;
  (void)value;
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK";
  }
  return false;
#else
  const int ret = MV_CC_SetIntValue(state_->handle, key.c_str(), static_cast<unsigned int>(value));
  if (ret != MV_OK) {
    return state_->Fail(ret, "SetInt " + key, error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::SetFloat(const std::string& key, double value, std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)key;
  (void)value;
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK";
  }
  return false;
#else
  const int ret = MV_CC_SetFloatValue(state_->handle, key.c_str(), static_cast<float>(value));
  if (ret != MV_OK) {
    return state_->Fail(ret, "SetFloat " + key, error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::SetEnum(const std::string& key, const std::string& value, std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)key;
  (void)value;
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK";
  }
  return false;
#else
  const int ret = MV_CC_SetEnumValueByString(state_->handle, key.c_str(), value.c_str());
  if (ret != MV_OK) {
    return state_->Fail(ret, "SetEnum " + key, error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::SetBool(const std::string& key, bool value, std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)key;
  (void)value;
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK";
  }
  return false;
#else
  const int ret = MV_CC_SetBoolValue(state_->handle, key.c_str(), value);
  if (ret != MV_OK) {
    return state_->Fail(ret, "SetBool " + key, error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::SetString(const std::string& key, const std::string& value, std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)key;
  (void)value;
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK";
  }
  return false;
#else
  const int ret = MV_CC_SetStringValue(state_->handle, key.c_str(), value.c_str());
  if (ret != MV_OK) {
    return state_->Fail(ret, "SetString " + key, error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::SetCommand(const std::string& key, std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)key;
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK";
  }
  return false;
#else
  const int ret = MV_CC_SetCommandValue(state_->handle, key.c_str());
  if (ret != MV_OK) {
    return state_->Fail(ret, "SetCommand " + key, error);
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::GetInt(const std::string& key, int64_t* value, std::string* error) {
#if !defined(INDUSTRIAL_CAMERA_HAS_MVS)
  (void)key;
  (void)value;
  if (error != nullptr) {
    *error = "未链接海康 MVS SDK";
  }
  return false;
#else
  MVCC_INTVALUE current;
  std::memset(&current, 0, sizeof(current));
  const int ret = MV_CC_GetIntValue(state_->handle, key.c_str(), &current);
  if (ret != MV_OK) {
    return state_->Fail(ret, "GetInt " + key, error);
  }
  if (value != nullptr) {
    *value = current.nCurValue;
  }
  state_->last_code = MV_OK;
  return true;
#endif
}

bool HikCameraPlugin::GetPayloadSize(uint64_t* bytes, std::string* error) {
  int64_t value = 0;
  if (!GetInt("PayloadSize", &value, error)) {
    return false;
  }
  if (bytes != nullptr) {
    *bytes = static_cast<uint64_t>(value);
  }
  return true;
}

int HikCameraPlugin::LastSdkCode() const { return state_->last_code; }

}  // namespace industrial_camera
