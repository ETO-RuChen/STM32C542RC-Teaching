[CmdletBinding()]
param([ValidateSet('Generate','Build','Flash')][string]$Action='Build',[switch]$Fresh)
$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
$lab=Join-Path $root 'labs/12-advanced-timer'
$src=Join-Path $lab 'LAB012_AdvancedTimer_cmake'
$ioc=Join-Path $lab 'LAB012_AdvancedTimer.ioc2'
$preset='debug_GCC_NUCLEO-C542RC'
$elf=Join-Path $src "build/$preset/LAB012_AdvancedTimer.elf"
function Invoke-Cube([string[]]$Arguments,[string]$WorkingDirectory=$root) {
  Push-Location $WorkingDirectory
  try { & cube @Arguments; if($LASTEXITCODE -ne 0){throw "cube failed: $($Arguments -join ' ')"} }
  finally { Pop-Location }
}
if($Action -eq 'Generate') {
  if(!(Test-Path (Join-Path $src 'app/main.c'))){throw 'Generate requires checked-in app and CMake integration'}
  $staging=Join-Path $root ('out/lab012-export-'+[guid]::NewGuid().ToString('N'))
  Invoke-Cube @('mx','ide-project','generate','--project-path',$ioc,'--format','CMake','--source','include-packs-from-local','--destination',$staging,'--diff','overwrite','--build-target','LAB012_AdvancedTimer.debug_GCC+NUCLEO-C542RC')
  Get-ChildItem -LiteralPath $staging -Force | Where-Object Name -ne 'CMakeLists.txt' | ForEach-Object {
    Copy-Item -LiteralPath $_.FullName -Destination $src -Recurse -Force
  }
  Write-Output "Fresh export retained: $staging"
}
$configure=@('cmake','--preset',$preset)
if($Fresh){$configure+='--fresh'}
Invoke-Cube $configure $src
Invoke-Cube @('cmake','--build','--preset',$preset,'--parallel','8') $src
if(!(Test-Path $elf)){throw "Missing ELF: $elf"}
if($Action -eq 'Flash') {
  $listing=& cube programmer -l stlink 2>&1
  if($LASTEXITCODE -ne 0){throw 'ST-LINK enumeration failed'}
  $probes=@([regex]::Matches(($listing -join "`n"),'ST-LINK SN\s*:\s*(\S+)'))
  if($probes.Count -ne 1){throw 'Expected exactly one ST-LINK'}
  Invoke-Cube @('programmer','-c','port=SWD',"sn=$($probes[0].Groups[1].Value)",'ap=1','mode=UR','reset=HWrst','freq=1000','-w',$elf,'-v','-rst')
}
