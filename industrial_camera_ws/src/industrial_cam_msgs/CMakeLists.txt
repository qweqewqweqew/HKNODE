cmake_minimum_required(VERSION 3.16)
project(industrial_cam_msgs)

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/ShmImageFrame.msg"
  "msg/TriggerCaptureCmd.msg"
  "msg/AlgorithmParamTune.msg"
  "srv/SetCameraParam.srv"
  DEPENDENCIES std_msgs
)

ament_export_dependencies(rosidl_default_runtime)
ament_package()
