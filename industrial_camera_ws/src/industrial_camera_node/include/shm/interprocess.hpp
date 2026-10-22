#pragma once

// Windows file-mapping subset of the Boost.Interprocess calls this project uses:
// shared_memory_object::remove, create_only, open_only, truncate, mapped_region.
// When INDUSTRIAL_CAMERA_HAS_BOOST is set, the real Boost headers are used instead.

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>

namespace boost {
namespace interprocess {

struct create_only_t {};
struct open_only_t {};
inline constexpr create_only_t create_only{};
inline constexpr open_only_t open_only{};

enum mode_t { read_write = 0 };

using offset_t = std::int64_t;

class interprocess_exception : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class shared_memory_object {
 public:
  static bool remove(const char* name) {
    const auto path = PathFor(name);
    std::error_code ec;
    const bool existed = std::filesystem::exists(path, ec);
    std::filesystem::remove(path, ec);
    return existed && !ec;
  }

  shared_memory_object(create_only_t, const char* name, mode_t) {
    OpenFile(name, true);
  }

  shared_memory_object(open_only_t, const char* name, mode_t) { OpenFile(name, false); }

  ~shared_memory_object() {
    if (file_ != INVALID_HANDLE_VALUE) {
      CloseHandle(file_);
    }
  }

  shared_memory_object(const shared_memory_object&) = delete;
  shared_memory_object& operator=(const shared_memory_object&) = delete;

  void truncate(offset_t size) {
    LARGE_INTEGER length;
    length.QuadPart = size;
    if (!SetFilePointerEx(file_, length, nullptr, FILE_BEGIN) || !SetEndOfFile(file_)) {
      throw interprocess_exception("设置共享内存文件大小失败");
    }
    size_ = static_cast<std::uint64_t>(size);
  }

  HANDLE handle() const { return file_; }
  std::uint64_t size() const { return size_; }

 private:
  static std::filesystem::path Directory() {
    wchar_t buffer[MAX_PATH];
    const DWORD n = GetTempPathW(MAX_PATH, buffer);
    std::filesystem::path dir = n > 0 ? std::filesystem::path(buffer) : std::filesystem::temp_directory_path();
    dir /= "industrial_camera_shm";
    std::filesystem::create_directories(dir);
    return dir;
  }

  static std::filesystem::path PathFor(const char* name) {
    if (name == nullptr || name[0] == '\0') {
      throw interprocess_exception("共享内存名为空");
    }
    return Directory() / name;
  }

  void OpenFile(const char* name, bool create) {
    path_ = PathFor(name);
    file_ = CreateFileW(path_.c_str(), GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr,
                        create ? CREATE_NEW : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file_ == INVALID_HANDLE_VALUE) {
      throw interprocess_exception(create ? "create_only 失败，共享内存已存在或无法创建"
                                         : "open_only 失败，共享内存不存在");
    }
    if (!create) {
      LARGE_INTEGER length;
      if (!GetFileSizeEx(file_, &length)) {
        throw interprocess_exception("读取共享内存大小失败");
      }
      size_ = static_cast<std::uint64_t>(length.QuadPart);
    }
  }

  HANDLE file_{INVALID_HANDLE_VALUE};
  std::filesystem::path path_;
  std::uint64_t size_{0};
};

class mapped_region {
 public:
  mapped_region(shared_memory_object& shm, mode_t) {
    const std::uint64_t size = shm.size();
    mapping_ = CreateFileMappingW(shm.handle(), nullptr, PAGE_READWRITE,
                                  static_cast<DWORD>(size >> 32), static_cast<DWORD>(size), nullptr);
    if (mapping_ == nullptr) {
      throw interprocess_exception("CreateFileMapping 失败");
    }
    view_ = MapViewOfFile(mapping_, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (view_ == nullptr) {
      CloseHandle(mapping_);
      mapping_ = nullptr;
      throw interprocess_exception("MapViewOfFile 失败");
    }
    size_ = static_cast<std::size_t>(size);
  }

  ~mapped_region() {
    if (view_ != nullptr) {
      UnmapViewOfFile(view_);
    }
    if (mapping_ != nullptr) {
      CloseHandle(mapping_);
    }
  }

  mapped_region(const mapped_region&) = delete;
  mapped_region& operator=(const mapped_region&) = delete;

  void* get_address() const { return view_; }
  std::size_t get_size() const { return size_; }

 private:
  HANDLE mapping_{nullptr};
  void* view_{nullptr};
  std::size_t size_{0};
};

}  // namespace interprocess
}  // namespace boost
