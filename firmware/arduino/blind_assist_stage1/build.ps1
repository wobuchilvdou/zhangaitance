param()
$root = (Resolve-Path "$PSScriptRoot\..\..\..\..").Path
$Cli = Join-Path $root ".tools\arduino-cli\arduino-cli.exe"
$cfg = Join-Path $root ".tools\arduino-data\cli.yaml"
$fqbn = "esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi"
$out = Join-Path $root "build"
$buildCache = Join-Path $env:LOCALAPPDATA "arduino\sketches\1FD9C3683FA7AEA446F7979207CAB2BC"
if (Test-Path $buildCache) {
    Remove-Item -Recurse -Force $buildCache
}
& $Cli --config-file $cfg compile --fqbn $fqbn --output-dir $out $PSScriptRoot
