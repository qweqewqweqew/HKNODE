#include "common/camera_logger.hpp"

#include <chrono>
#include <condition_variable>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <queue>
#include <sstream>
#include <thread>
#include <unordered_map>

namespace industrial_camera {
namespace {

std::string Timestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) %
                      1000;
  const std::time_t seconds = std::chrono::system_clock::to_time_t(now);
  std::tm local_tm{};
  localtime_s(&local_tm, &seconds);
  std::ostringstream out;
  out << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << '.' << std::setw(3) << std::setfill('0')
      << millis.count();
  return out.str();
}

}  // namespace

struct CameraLogger::Impl {
  std::mutex mutex;
  std::condition_variable cv;
  std::queue<std::string> lines;
  std::unordered_map<std::string, std::chrono::steady_clock::time_point> throttle;
  std::function<void(const std::string&)> sink;
  std::ofstream file;
  std::filesystem::path path;
  std::thread worker;
  bool stop{false};

  ~Impl() { Stop(); }

  void Stop() {
    {
      std::lock_guard<std::mutex> lock(mutex);
      stop = true;
    }
    cv.notify_all();
    if (worker.joinable()) {
      worker.join();
    }
    if (file.is_open()) {
      file.flush();
      file.close();
    }
  }

  void Run() {
    for (;;) {
      std::string line;
      {
        std::unique_lock<std::mutex> lock(mutex);
        cv.wait(lock, [&] { return stop || !lines.empty(); });
        if (lines.empty() && stop) {
          return;
        }
        line = std::move(lines.front());
        lines.pop();
      }
      if (file.is_open()) {
        file << line << '\n';
        file.flush();
        if (file.tellp() > 8 * 1024 * 1024) {
          file.close();
          std::filesystem::path older = path;
          older += ".1";
          std::error_code ec;
          std::filesystem::rename(path, older, ec);
          file.open(path, std::ios::app);
        }
      }
    }
  }
};

CameraLogger::~CameraLogger() { Close(); }

bool CameraLogger::Open(const std::string& directory, std::string* error) {
  Close();
  impl_ = new Impl();
  std::error_code ec;
  std::filesystem::create_directories(directory, ec);
  if (ec) {
    if (error != nullptr) {
      *error = "创建日志目录失败: " + directory;
    }
    delete impl_;
    impl_ = nullptr;
    return false;
  }
  impl_->path = std::filesystem::path(directory) / "camera_node.log";
  impl_->file.open(impl_->path, std::ios::app);
  if (!impl_->file) {
    if (error != nullptr) {
      *error = "打开日志文件失败: " + impl_->path.string();
    }
    delete impl_;
    impl_ = nullptr;
    return false;
  }
  impl_->worker = std::thread([this] { impl_->Run(); });
  return true;
}

void CameraLogger::Close() {
  delete impl_;
  impl_ = nullptr;
}

void CameraLogger::Log(const std::string& level, const std::string& station,
                       const std::string& message) {
  if (impl_ == nullptr) {
    return;
  }
  std::ostringstream out;
  out << '[' << Timestamp() << "][" << level << "][" << station << "] " << message;
  const std::string line = out.str();
  {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    if (impl_->sink) {
      impl_->sink(line);
    }
    impl_->lines.push(line);
  }
  impl_->cv.notify_one();
}

void CameraLogger::Info(const std::string& station, const std::string& message) {
  Log("INFO", station, message);
}

void CameraLogger::Warn(const std::string& station, const std::string& message) {
  Log("WARN", station, message);
}

void CameraLogger::Error(const std::string& station, const std::string& message) {
  Log("ERROR", station, message);
}

void CameraLogger::Stat(const std::string& station, const std::string& body) {
  if (impl_ == nullptr) {
    return;
  }
  std::ostringstream out;
  out << '[' << Timestamp() << "][INFO][STAT][" << station << "] " << body;
  const std::string line = out.str();
  {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    if (impl_->sink) {
      impl_->sink(line);
    }
    impl_->lines.push(line);
  }
  impl_->cv.notify_one();
}

void CameraLogger::Throttle(const std::string& key, const std::string& level,
                            const std::string& station, const std::string& message) {
  if (impl_ == nullptr) {
    return;
  }
  const auto now = std::chrono::steady_clock::now();
  {
    std::lock_guard<std::mutex> lock(impl_->mutex);
    const auto found = impl_->throttle.find(key);
    if (found != impl_->throttle.end() && now - found->second < std::chrono::seconds(1)) {
      return;
    }
    impl_->throttle[key] = now;
  }
  Log(level, station, message);
}

void CameraLogger::SetSinkForTest(std::function<void(const std::string&)> sink) {
  if (impl_ == nullptr) {
    impl_ = new Impl();
  }
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->sink = std::move(sink);
}

}  // namespace industrial_camera
