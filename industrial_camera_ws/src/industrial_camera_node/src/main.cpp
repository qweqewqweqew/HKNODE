#include <rclcpp/rclcpp.hpp>

#include <memory>
#include <string>
#include <unordered_map>

#include "industrial_cam_msgs/msg/algorithm_param_tune.hpp"
#include "industrial_cam_msgs/msg/shm_image_frame.hpp"
#include "industrial_cam_msgs/msg/trigger_capture_cmd.hpp"
#include "industrial_cam_msgs/srv/set_camera_param.hpp"
#include "node/camera_manager_node.hpp"

namespace {

class CameraManagerNode : public rclcpp::Node {
 public:
  CameraManagerNode() : rclcpp::Node("camera_manager_node"), runtime_(logger_) {
    declare_parameter<std::string>("topology_file", "");
  }

  bool Start(std::string* error) {
    const std::string topology = get_parameter("topology_file").as_string();
    if (topology.empty()) {
      if (error != nullptr) {
        *error = "缺少参数 topology_file";
      }
      return false;
    }
    runtime_.SetNotifier([this](const industrial_camera::FrameNotice& notice) { Publish(notice); });
    if (!runtime_.Start(topology, industrial_camera::CreateHikDevice, error)) {
      return false;
    }
    const auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable();
    trigger_sub_ = create_subscription<industrial_cam_msgs::msg::TriggerCaptureCmd>(
        runtime_.Config().trigger_cmd_topic, qos,
        [this](const industrial_cam_msgs::msg::TriggerCaptureCmd::SharedPtr msg) {
          industrial_camera::TriggerRequest request;
          request.camera_id = msg->camera_id;
          request.trigger_seq_id = msg->trigger_seq_id;
          request.workpiece_id = msg->workpiece_id;
          request.override_exposure_us = msg->override_exposure_us;
          request.override_gain_db = msg->override_gain_db;
          std::string failure;
          if (!runtime_.SubmitTrigger(request, &failure)) {
            RCLCPP_ERROR(get_logger(), "%s", failure.c_str());
          }
        });
    tune_sub_ = create_subscription<industrial_cam_msgs::msg::AlgorithmParamTune>(
        runtime_.Config().algo_tune_topic, qos,
        [this](const industrial_cam_msgs::msg::AlgorithmParamTune::SharedPtr msg) {
          industrial_camera::TuneRequest request;
          request.camera_id = msg->camera_id;
          request.base_frame_id = msg->base_frame_id;
          request.base_param_version = msg->base_param_version;
          request.tune_exposure = msg->tune_exposure;
          request.target_exposure_us = msg->target_exposure_us;
          request.tune_gain = msg->tune_gain;
          request.target_gain_db = msg->target_gain_db;
          request.retrigger_immediately = msg->retrigger_immediately;
          request.retrigger_seq_id = msg->retrigger_seq_id;
          request.workpiece_id = msg->workpiece_id;
          std::string failure;
          if (!runtime_.SubmitTune(request, &failure)) {
            RCLCPP_WARN(get_logger(), "%s", failure.c_str());
          }
        });
    service_ = create_service<industrial_cam_msgs::srv::SetCameraParam>(
        runtime_.Config().set_param_service,
        [this](const std::shared_ptr<industrial_cam_msgs::srv::SetCameraParam::Request> request,
               std::shared_ptr<industrial_cam_msgs::srv::SetCameraParam::Response> response) {
          industrial_camera::ParamWriteRequest write;
          write.camera_id = request->camera_id;
          write.key = request->param_key;
          write.type = static_cast<industrial_camera::ParamType>(request->param_type);
          write.value = request->param_value;
          const auto result = runtime_.SetParam(write);
          response->success = result.success;
          response->new_param_version_seq = result.new_param_version_seq;
          response->sdk_error_code = result.sdk_error_code;
          response->message = result.message;
        });
    for (const auto& station : runtime_.Config().stations) {
      publishers_[station.name] =
          create_publisher<industrial_cam_msgs::msg::ShmImageFrame>(station.topic_name, qos);
    }
    return true;
  }

  void StopNode() { runtime_.Stop(); }

 private:
  void Publish(const industrial_camera::FrameNotice& notice) {
    const auto found = publishers_.find(notice.camera_id);
    if (found == publishers_.end()) {
      return;
    }
    industrial_cam_msgs::msg::ShmImageFrame message;
    message.header.stamp = rclcpp::Time(notice.stamp_system_ns);
    message.header.frame_id = notice.optical_frame_id;
    message.camera_id = notice.camera_id;
    message.capture_mode = notice.capture_mode;
    message.trigger_seq_id = notice.trigger_seq_id;
    message.workpiece_id = notice.workpiece_id;
    message.param_version_seq = notice.param_version_seq;
    message.shm_name = notice.shm_name;
    message.slot_index = notice.slot_index;
    message.data_offset = notice.data_offset;
    message.data_size = notice.data_size;
    message.frame_id = notice.frame_id;
    message.width = notice.width;
    message.height = notice.height;
    message.step = notice.step;
    message.pixel_format = notice.pixel_format;
    message.exposure_time_us = notice.exposure_time_us;
    message.gain_db = notice.gain_db;
    found->second->publish(message);
  }

  industrial_camera::CameraLogger logger_;
  industrial_camera::CameraRuntime runtime_;
  rclcpp::Subscription<industrial_cam_msgs::msg::TriggerCaptureCmd>::SharedPtr trigger_sub_;
  rclcpp::Subscription<industrial_cam_msgs::msg::AlgorithmParamTune>::SharedPtr tune_sub_;
  rclcpp::Service<industrial_cam_msgs::srv::SetCameraParam>::SharedPtr service_;
  std::unordered_map<std::string,
                     rclcpp::Publisher<industrial_cam_msgs::msg::ShmImageFrame>::SharedPtr>
      publishers_;
};

}  // namespace

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CameraManagerNode>();
  std::string error;
  if (!node->Start(&error)) {
    RCLCPP_ERROR(rclcpp::get_logger("camera_manager_node"), "%s", error.c_str());
    rclcpp::shutdown();
    return 2;
  }
  rclcpp::spin(node);
  node->StopNode();
  rclcpp::shutdown();
  return 0;
}
