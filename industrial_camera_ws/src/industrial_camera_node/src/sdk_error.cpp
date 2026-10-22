#include "common/sdk_error.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

namespace industrial_camera {
namespace {

const std::unordered_map<int, const char*>& ErrorTable() {
  static const std::unordered_map<int, const char*> table = {
      {0x00000000, "MV_OK 成功"},
      {static_cast<int>(0x80000000), "MV_E_HANDLE 无效句柄"},
      {static_cast<int>(0x80000001), "MV_E_SUPPORT 不支持的功能"},
      {static_cast<int>(0x80000002), "MV_E_BUFOVER 缓存已满"},
      {static_cast<int>(0x80000003), "MV_E_CALLORDER 调用顺序错误"},
      {static_cast<int>(0x80000004), "MV_E_PARAMETER 参数错误"},
      {static_cast<int>(0x80000006), "MV_E_RESOURCE 资源申请失败"},
      {static_cast<int>(0x80000007), "MV_E_NODATA 无数据"},
      {static_cast<int>(0x80000008), "MV_E_PRECONDITION 前置条件不满足"},
      {static_cast<int>(0x80000009), "MV_E_VERSION 版本不匹配"},
      {static_cast<int>(0x8000000A), "MV_E_NOENOUGH_BUF 缓存不足"},
      {static_cast<int>(0x8000000B), "MV_E_ABNORMAL_IMAGE 异常图像"},
      {static_cast<int>(0x8000000C), "MV_E_LOAD_LIBRARY 动态库加载失败"},
      {static_cast<int>(0x800000FF), "MV_E_UNKNOW 未知错误"},
      {static_cast<int>(0x80000100), "MV_E_GC_GENERIC GenICam 通用错误"},
      {static_cast<int>(0x80000102), "MV_E_GC_RANGE 参数超出范围"},
      {static_cast<int>(0x80000106), "MV_E_GC_ACCESS 属性不可访问"},
      {static_cast<int>(0x80000107), "MV_E_GC_TIMEOUT 属性访问超时"},
      {static_cast<int>(0x80000203), "MV_E_ACCESS_DENIED 访问被拒绝"},
      {static_cast<int>(0x80000204), "MV_E_BUSY 设备忙"},
      {static_cast<int>(0x80000205), "MV_E_PACKET 网络包异常"},
      {static_cast<int>(0x80000206), "MV_E_NETER 网络错误"},
  };
  return table;
}

}  // namespace

std::string TranslateSdkError(int code) {
  const auto& table = ErrorTable();
  const auto found = table.find(code);
  std::ostringstream out;
  out << "0x" << std::hex << static_cast<unsigned int>(code);
  if (found != table.end()) {
    return std::string(found->second) + " (" + out.str() + ")";
  }
  return "未收录的海康错误码 (" + out.str() + ")";
}

}  // namespace industrial_camera
