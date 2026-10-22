cmake_minimum_required(VERSION 3.16)
project(industrial_camera_node LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(camera_core STATIC
  src/shm_ring_buffer.cpp
  src/trigger_token_queue.cpp
  src/auto_tune_controller.cpp
  src/sdk_error.cpp
  src/yaml_config.cpp
  src/config_validator.cpp
  src/camera_logger.cpp
  src/hik_camera_plugin.cpp
  src/camera_station_worker.cpp
  src/camera_manager_node.cpp
)
target_include_directories(camera_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(camera_core PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
if(MSVC)
  target_compile_options(camera_core PRIVATE /utf-8 /W3 /EHsc)
endif()

find_path(MVS_INCLUDE_DIR NAMES MvCameraControl.h
  HINTS "$ENV{MVS_DEV_DIR}"
  PATHS "C:/Program Files (x86)/MVS/Development" "C:/Program Files/MVS/Development"
  PATH_SUFFIXES Includes)
find_library(MVS_LIBRARY NAMES MvCameraControl
  HINTS "$ENV{MVS_DEV_DIR}"
  PATHS
    "C:/Program Files (x86)/MVS/Development/Libraries/win64"
    "C:/Program Files/MVS/Development/Libraries/win64")
if(MVS_INCLUDE_DIR AND MVS_LIBRARY)
  target_compile_definitions(camera_core PUBLIC INDUSTRIAL_CAMERA_HAS_MVS)
  target_include_directories(camera_core PUBLIC ${MVS_INCLUDE_DIR})
  target_link_libraries(camera_core PUBLIC ${MVS_LIBRARY})
  message(STATUS "海康 MVS SDK: ${MVS_LIBRARY}")
else()
  message(STATUS "未找到海康 MVS SDK，节点可编译但无法打开真机")
endif()

find_package(ament_cmake QUIET)
if(ament_cmake_FOUND)
  find_package(rclcpp REQUIRED)
  find_package(industrial_cam_msgs REQUIRED)
  add_executable(camera_manager_node src/main.cpp)
  target_compile_definitions(camera_manager_node PRIVATE INDUSTRIAL_CAMERA_HAS_ROS)
  target_link_libraries(camera_manager_node PRIVATE camera_core)
  ament_target_dependencies(camera_manager_node rclcpp industrial_cam_msgs)
  if(MSVC)
    target_compile_options(camera_manager_node PRIVATE /utf-8 /W3 /EHsc)
  endif()
  install(TARGETS camera_manager_node DESTINATION lib/${PROJECT_NAME})
  install(DIRECTORY config DESTINATION share/${PROJECT_NAME})
  install(DIRECTORY include/ DESTINATION include)
  ament_package()
endif()

option(INDUSTRIAL_CAMERA_BUILD_TESTS "Build camera core tests" ON)
if(INDUSTRIAL_CAMERA_BUILD_TESTS)
  enable_testing()
  file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" CAMERA_TEST_SOURCE_DIR)
  add_executable(camera_core_tests tests/test_core.cpp)
  target_link_libraries(camera_core_tests PRIVATE camera_core)
  target_compile_definitions(camera_core_tests PRIVATE "TEST_SOURCE_DIR=\"${CAMERA_TEST_SOURCE_DIR}\"")
  if(MSVC)
    target_compile_options(camera_core_tests PRIVATE /utf-8 /W3 /EHsc)
  endif()
  add_test(NAME camera_core_tests COMMAND camera_core_tests)
endif()
