# HKNODE

基于 ROS 2 的海康网络相机可复用节点。需求基线是《需求规格说明书 V1.0》（2026-10-22），技术参考是《架构设计 V4.0》。两边冲突时以需求文档为准。

本仓库当前验收等级：**条件通过（Pending Hardware）**。无海康真机，T1/T2 已在 Windows 上跑通，T3/T4 见 `docs/测试文档.md` 的真机补测清单。

## 环境前提

- Windows 10/11 x64，小端
- Visual Studio 2022 或 2026，含 MSVC C++17 工具集
- CMake 3.16 以上（Visual Studio 自带即可）
- 真机运行另需：ROS 2（Windows 发行版）、海康 MVS SDK、GigE/USB3 相机
- 共享内存名不能包含 `\` 或 `/`

## 依赖

| 组件 | 用途 | 本仓库做法 |
| --- | --- | --- |
| C++17 | 节点与共享内存 | MSVC |
| ROS 2 rclcpp | 控制面消息 | 安装 ROS 后用 colcon 编译 `industrial_cam_msgs` 与节点 |
| 海康 MVS | 打开相机 | 设置 `MVS_DEV_DIR` 指向 Development 目录后重编 |
| 共享内存 | 图像零拷贝 | 调用 `shared_memory_object::remove` + `create_only` + `mapped_region`。未安装 Boost 时使用 `include/shm/interprocess.hpp` 里的 Windows 文件映射，语义与上述三个调用一致 |

## 快速开始

在仓库根目录：

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\deploy.ps1
powershell -ExecutionPolicy Bypass -File .\scripts\rebuild.ps1
```

`rebuild.ps1` 会清理 `build/`，编译核心库并运行 `camera_core_tests.exe`。失败时进程退出码非 0。

有 ROS 2 时，在已执行 ROS 的 `local_setup.ps1` 的终端里：

```powershell
cd industrial_camera_ws
colcon build --packages-select industrial_cam_msgs industrial_camera_node
powershell -ExecutionPolicy Bypass -File ..\scripts\start.ps1
```

启动参数可以改配置路径：

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\start.ps1 -Topology D:\path\project_topology.yaml
```

进程会打印 `trigger_cmd_topic`、`algo_tune_topic`、`set_param_service` 以及每个工位的图像 topic 和共享内存名。

## 配置说明

两层 YAML：

1. `industrial_camera_ws/src/industrial_camera_node/config/project_topology.yaml`  
   工位、模式、共享内存、钳位、`param_settle_us`、`trigger_queue_depth`。
2. 每个工位的 `feature_config_file`。键名固定为 `feature`、`type`、`value`。`type` 只能是 `int`、`float`、`enum`、`bool`、`string`、`cmd`。

已确认的实现约定：

- 软触发先入队再 `TriggerSoftware`，默认队列深度 1。超时后的迟到帧记为孤儿帧，不绑到下一次命令。
- 硬触发不走这把队列，`trigger_seq_id` 为 0。
- 一令一参写成功后成为新的当前曝光/增益，不在出图后自动改回。
- 调参合并槽在覆盖前比较 `base_param_version`。
- 图像回调和异常回调只置标志；重连在工位线程里拿 `sdk_mutex` 完成。
- 触发、调参、出图通知使用 reliable、深度 10。

`param_settle_us` 默认 0。经验区间写在拓扑文件注释里：GigE 0~1000；关闭 Auto 或大跨度 2000~5000；USB3 多为 0；线扫或老 GigE 先 5000 再往下压。

## 常见故障

| 现象 | 处理 |
| --- | --- |
| 启动日志含「预检失败: PayloadSize」 | 增大 `shm_slot_capacity_mb`，或降低分辨率/像素格式 |
| 「未找到相机」 | 核对序列号或 IP；MVS 客户端能打开后再启动节点 |
| 「本构建未链接海康 MVS SDK」 | 安装 MVS，设置 `MVS_DEV_DIR`，重新运行 `rebuild.ps1` 或 colcon |
| STAT 里「触发超时未出图」增加 | 触发间隔小于曝光加读出时间，或硬触发线路不通 |
| STAT 里「SHM写满丢帧」增加 | 下游太慢，或加大 `shm_slot_count` |
| STAT 里「SHM超时强占」增加 | 下游崩溃或卡死；按最慢消费者 P99 的 1.5~2 倍设置 `shm_lock_timeout_ms` |
| GigE 丢包 | 网卡巨型帧 9014，并调大 `GevSCPSPacketSize` |

日志在拓扑里的 `log_dir`，默认 `./logs/camera_node/camera_node.log`。每 10 秒一条 `[STAT]`。

## 目录

```text
industrial_camera_ws/src/industrial_cam_msgs
industrial_camera_ws/src/industrial_camera_node
docs/交接文档.md
docs/测试文档.md
scripts/deploy.ps1
scripts/rebuild.ps1
scripts/start.ps1
```

下游零拷贝读取入口：`industrial_camera_node/include/shm/shm_image_client.hpp`。
