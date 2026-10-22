$HknodeRosSetup = "C:\opt\ros\foxy\x64\local_setup.bat"

function Import-CmdEnvironment {
    param([string]$SetupBat)
    if (-not (Test-Path -LiteralPath $SetupBat)) {
        Write-Output "MISSING: $SetupBat"
        return $false
    }
    $lines = cmd /c "call `"$SetupBat`" >nul && set"
    foreach ($line in $lines) {
        $eq = $line.IndexOf("=")
        if ($eq -lt 1) { continue }
        $name = $line.Substring(0, $eq)
        if ($name -notmatch '^[A-Za-z_][A-Za-z0-9_]*$') { continue }
        if ($name -match '^(COMPUTERNAME|USERNAME|USERDOMAIN|USERDOMAIN_ROAMINGPROFILE|LOGONSERVER|USERPROFILE|HOMEDRIVE|HOMEPATH)$') { continue }
        Set-Item -Path "Env:$name" -Value $line.Substring($eq + 1)
    }
    return $true
}

function Import-HknodeRos {
    return Import-CmdEnvironment -SetupBat $HknodeRosSetup
}
