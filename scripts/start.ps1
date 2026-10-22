param(
    [string]$Topology = ""
)

$ErrorActionPreference = "Stop"
$repo = Split-Path -Parent $PSScriptRoot
if (-not $Topology) {
    $Topology = Join-Path $repo "industrial_camera_ws\src\industrial_camera_node\config\project_topology.yaml"
}
if (-not (Test-Path $Topology)) {
    Write-Output "topology not found: $Topology"
    exit 1
}

$ros2 = Get-Command ros2 -ErrorAction SilentlyContinue
if (-not $ros2) {
    Write-Output "ros2 is not on PATH. Open a ROS 2 terminal and run this script again."
    Write-Output "topology=$Topology"
    Select-String -Path $Topology -Pattern "topic:|_topic:|service:" | ForEach-Object { $_.Line.Trim() }
    exit 1
}

Write-Output "topology=$Topology"
Select-String -Path $Topology -Pattern "topic:|_topic:" | ForEach-Object { $_.Line.Trim() }
& ros2 run industrial_camera_node camera_manager_node --ros-args -p "topology_file:=$Topology"
exit $LASTEXITCODE
