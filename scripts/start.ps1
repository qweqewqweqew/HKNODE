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

. (Join-Path $PSScriptRoot "use_foxy.ps1")
if (-not (Import-HknodeRos)) { exit 1 }
$workspaceSetup = Join-Path $repo "industrial_camera_ws\install\local_setup.bat"
if (-not (Test-Path -LiteralPath $workspaceSetup)) {
    Write-Output "workspace install not found. Run scripts\rebuild.ps1 first."
    exit 1
}
if (-not (Import-CmdEnvironment -SetupBat $workspaceSetup)) { exit 1 }
$ros2 = Get-Command ros2 -ErrorAction SilentlyContinue
if (-not $ros2) {
    Write-Output "ros2 is not on PATH after sourcing $HknodeRosSetup"
    exit 1
}

Write-Output "topology=$Topology"
Select-String -Path $Topology -Pattern "topic:|_topic:" | ForEach-Object { $_.Line.Trim() }
& ros2 run industrial_camera_node camera_manager_node --ros-args -p "topology_file:=$Topology"
exit $LASTEXITCODE
