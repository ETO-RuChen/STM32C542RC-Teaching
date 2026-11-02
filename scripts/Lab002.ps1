[CmdletBinding()]
param(
    [ValidateSet('Generate', 'Build', 'Flash')]
    [string]$Action = 'Build',
    [switch]$Fresh
)
$ErrorActionPreference = 'Stop'
$repositoryRoot = Split-Path -Parent $PSScriptRoot
$labRoot = Join-Path $repositoryRoot 'labs/02-uart-logging'
$projectFile = Join-Path $labRoot 'LAB002_UartLogging.ioc2'
$sourceRoot = Join-Path $labRoot 'LAB002_UartLogging_cmake'
$preset = 'debug_GCC_NUCLEO-C542RC'
$elfPath = Join-Path $sourceRoot "build/$preset/LAB002_UartLogging.elf"
function Invoke-Cube([string[]]$Arguments, [string]$WorkingDirectory = $repositoryRoot) {
    Push-Location -LiteralPath $WorkingDirectory
    try { & cube @Arguments; if ($LASTEXITCODE -ne 0) { throw "cube failed (exit $LASTEXITCODE): $($Arguments -join ' ')" } }
    finally { Pop-Location }
}
function Invoke-Generate {
    Invoke-Cube @('mx','ide-project','generate','--project-path',$projectFile,'--format','CMake','--source','include-packs-from-local','--destination',$sourceRoot,'--force','--diff','keep_user','--build-target','LAB002_UartLogging.debug_GCC+NUCLEO-C542RC')
    foreach ($file in @('generated/hal/mx_usart2.c','generated/hal/mx_usart2.h','generated/hal/mx_system.c')) { if (-not (Test-Path -LiteralPath (Join-Path $sourceRoot $file))) { throw "Missing generated file: $file" } }
}
function Invoke-Build {
    $configure = @('cmake','--preset',$preset); if ($Fresh) { $configure += '--fresh' }
    Invoke-Cube $configure $sourceRoot
    Invoke-Cube @('cmake','--build','--preset',$preset,'--parallel','8') $sourceRoot
    if (-not (Test-Path -LiteralPath $elfPath)) { throw "Missing ELF: $elfPath" }
}
function Get-UniqueProbeSerial {
    $listing = & cube programmer -l stlink 2>&1; if ($LASTEXITCODE -ne 0) { throw ($listing -join [Environment]::NewLine) }
    $matches = @([regex]::Matches(($listing -join [Environment]::NewLine), 'ST-LINK SN\s*:\s*(\S+)'))
    if ($matches.Count -ne 1) { throw 'Expected exactly one ST-LINK.' }; return $matches[0].Groups[1].Value
}
switch ($Action) {
    'Generate' { Invoke-Generate; Invoke-Build }
    'Build' { Invoke-Build }
    'Flash' { Invoke-Build; $probeSerial = Get-UniqueProbeSerial; Invoke-Cube @('programmer','-c','port=SWD',"sn=$probeSerial",'ap=1','mode=UR','reset=HWrst','freq=1000','-w',$elfPath,'-v','-rst') $sourceRoot }
}
