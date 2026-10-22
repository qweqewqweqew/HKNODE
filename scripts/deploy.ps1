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

. (Join-Path $PSScriptRoot "use_foxy.ps1")
if (Test-Path -LiteralPath $HknodeRosSetup) {
    Write-Output "OK: ROS 2 Foxy $HknodeRosSetup"
} else {
    Write-Output "MISSING: ROS 2 Foxy $HknodeRosSetup"
    exit 1
}

$nodeRoot = Join-Path $PSScriptRoot "..\industrial_camera_ws\src\industrial_camera_node\vendor\mvs"
$mvsHeader = Join-Path $nodeRoot "include\MvCameraControl.h"
$mvsLib = Join-Path $nodeRoot "lib\win64\MvCameraControl.lib"
$mvsDll = Join-Path $nodeRoot "bin\win64\MVCameraControl.dll"
if ((Test-Path $mvsHeader) -and (Test-Path $mvsLib)) {
    Write-Output "OK: MVS headers and import library under vendor/mvs"
} else {
    Write-Output "WARN: vendor/mvs is missing MvCameraControl.h or the win64 import library."
}
if (Test-Path $mvsDll) {
    Write-Output "OK: MVCameraControl.dll"
} else {
    Write-Output "WARN: vendor/mvs/bin/win64/MVCameraControl.dll is missing. The node can link, but it cannot load the camera SDK at runtime."
}

if ($env:BOOST_ROOT -and (Test-Path (Join-Path $env:BOOST_ROOT "boost\interprocess\shared_memory_object.hpp"))) {
    Write-Output "OK: BOOST_ROOT=$env:BOOST_ROOT"
} else {
    Write-Output "WARN: Boost.Interprocess headers not found. The Windows mapping shim will be used."
}

Write-Output "deploy check finished"
exit 0
