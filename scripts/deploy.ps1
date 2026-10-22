$ErrorActionPreference = "Stop"

Write-Output "HKNODE deploy check"

$vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    Write-Output "MISSING: Visual Studio Installer"
    exit 1
}

$cmake = & $vswhere -latest -find "Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" | Select-Object -First 1
if (-not $cmake) {
    Write-Output "MISSING: CMake"
    exit 1
}
Write-Output "OK: CMake $cmake"

$cl = & $vswhere -latest -find "VC\Tools\MSVC\*\bin\Hostx64\x64\cl.exe" | Select-Object -First 1
if (-not $cl) {
    Write-Output "MISSING: MSVC cl.exe"
    exit 1
}
Write-Output "OK: MSVC $cl"

if ($env:ROS_DISTRO) {
    Write-Output "OK: ROS_DISTRO=$env:ROS_DISTRO"
} else {
    Write-Output "WARN: ROS 2 is not configured. Core tests can build. The camera node executable needs colcon."
}

$mvsCandidates = @(
    $env:MVS_DEV_DIR,
    "C:\Program Files (x86)\MVS\Development",
    "C:\Program Files\MVS\Development"
) | Where-Object { $_ -and (Test-Path $_) }
if ($mvsCandidates) {
    Write-Output "OK: MVS $($mvsCandidates[0])"
} else {
    Write-Output "WARN: Hikvision MVS SDK not found. Set MVS_DEV_DIR before a hardware build."
}

if ($env:BOOST_ROOT -and (Test-Path (Join-Path $env:BOOST_ROOT "boost\interprocess\shared_memory_object.hpp"))) {
    Write-Output "OK: BOOST_ROOT=$env:BOOST_ROOT"
} else {
    Write-Output "WARN: Boost.Interprocess headers not found. The Windows mapping shim will be used."
}

Write-Output "deploy check finished"
exit 0
