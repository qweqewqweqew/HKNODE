$ErrorActionPreference = "Stop"
$repo = Split-Path -Parent $PSScriptRoot
Set-Location $repo

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
$cmake = & $vswhere -latest -find "Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" | Select-Object -First 1
if (-not $cmake) {
    Write-Output "cmake not found"
    exit 1
}

if (Test-Path (Join-Path $repo "build")) {
    Remove-Item -Recurse -Force (Join-Path $repo "build")
}

& $cmake -S $repo -B (Join-Path $repo "build") -G "Visual Studio 18 2026" -A x64
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

& $cmake --build (Join-Path $repo "build") --config Release --target camera_core_tests
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$exe = Get-ChildItem -Path (Join-Path $repo "build") -Recurse -Filter camera_core_tests.exe |
    Where-Object { $_.FullName -match "Release" } |
    Select-Object -First 1
if (-not $exe) {
    Write-Output "camera_core_tests.exe was not produced"
    exit 1
}

Write-Output "RUN $($exe.FullName)"
& $exe.FullName
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

. (Join-Path $PSScriptRoot "use_foxy.ps1")
if (-not (Import-HknodeRos)) { exit 1 }
Write-Output "ROS_DISTRO=$env:ROS_DISTRO"

$ws = Join-Path $repo "industrial_camera_ws"
foreach ($dir in @("build", "install", "log")) {
    $path = Join-Path $ws $dir
    if (Test-Path $path) {
        Remove-Item -Recurse -Force $path
    }
}
Set-Location $ws
$env:PYTHONUTF8 = "1"
$foxyPython = "C:\opt\ros\foxy\x64\python.exe"
& colcon build --merge-install --packages-select industrial_cam_msgs industrial_camera_node --cmake-args -G "Visual Studio 16 2019" -A x64 -DINDUSTRIAL_CAMERA_BUILD_TESTS=OFF "-DPython3_EXECUTABLE=$foxyPython" "-DPYTHON_EXECUTABLE=$foxyPython"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
Write-Output "rebuild finished"
exit 0
