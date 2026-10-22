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

set(MVS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/mvs")
set(MVS_INCLUDE_DIR "${MVS_ROOT}/include")
set(MVS_LIBRARY "${MVS_ROOT}/lib/win64/MvCameraControl.lib")
set(MVS_BIN_DIR "${MVS_ROOT}/bin/win64")
file(GLOB MVS_RUNTIME_FILES
  "${MVS_BIN_DIR}/*.dll"
  "${MVS_BIN_DIR}/*.cti"
  "${MVS_BIN_DIR}/*.ini"
  "${MVS_BIN_DIR}/*.manifest")
if(EXISTS "${MVS_INCLUDE_DIR}/MvCameraControl.h" AND EXISTS "${MVS_LIBRARY}")
  target_compile_definitions(camera_core PUBLIC INDUSTRIAL_CAMERA_HAS_MVS)
  target_include_directories(camera_core PUBLIC "${MVS_INCLUDE_DIR}")
  target_link_libraries(camera_core PUBLIC "${MVS_LIBRARY}")
  message(STATUS "海康 MVS SDK: ${MVS_LIBRARY}")
else()
  message(STATUS "未找到随项目提供的海康头文件或导入库，节点可编译但无法打开真机")
endif()

find_package(ament_cmake QUIET)
if(ament_cmake_FOUND)
  find_package(rclcpp REQUIRED)
  find_package(industrial_cam_msgs REQUIRED)
  add_executable(camera_manager_node src/main.cpp)
  target_compile_definitions(camera_manager_node PRIVATE INDUSTRIAL_CAMERA_HAS_ROS)
  ament_target_dependencies(camera_manager_node rclcpp industrial_cam_msgs)
  target_link_libraries(camera_manager_node camera_core)
  if(MSVC)
    target_compile_options(camera_manager_node PRIVATE /utf-8 /W3 /EHsc)
  endif()
  install(TARGETS camera_manager_node DESTINATION lib/${PROJECT_NAME})
  if(MVS_RUNTIME_FILES)
    install(FILES ${MVS_RUNTIME_FILES} DESTINATION lib/${PROJECT_NAME})
  endif()
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

if(MVS_RUNTIME_FILES)
  foreach(mvs_target camera_core_tests camera_manager_node)
    if(TARGET ${mvs_target})
      add_custom_command(TARGET ${mvs_target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${MVS_RUNTIME_FILES} "$<TARGET_FILE_DIR:${mvs_target}>"
        COMMENT "Copy Hikvision runtime DLLs next to ${mvs_target}")
    endif()
  endforeach()
endif()
