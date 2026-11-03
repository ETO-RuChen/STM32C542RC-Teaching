[CmdletBinding()]
param([ValidateSet('Generate','Build','Flash')][string]$Action='Build',[switch]$Fresh)
$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
$lab=Join-Path $root 'labs/06-cordic'
$src=Join-Path $lab 'LAB006_Cordic_cmake'
$ioc=Join-Path $lab 'LAB006_Cordic.ioc2'
$preset='debug_GCC_NUCLEO-C542RC'
$elf=Join-Path $src "build/$preset/LAB006_Cordic.elf"
function Invoke-Cube([string[]]$Arguments,[string]$WorkingDirectory=$root) {
  Push-Location $WorkingDirectory
  try { & cube @Arguments; if($LASTEXITCODE -ne 0){throw "cube command failed: $($Arguments -join ' ')"} }
  finally { Pop-Location }
}
if($Action -eq 'Generate') {
  Invoke-Cube @('mx','global-services','enable','HALCore','-p',$ioc)
  Invoke-Cube @('mx','global-services','enable','SystemInit','-p',$ioc)
  Invoke-Cube @('mx','ide-project','generate','--project-path',$ioc,'--format','CMake','--source','include-packs-from-local','--destination',$src,'--force','--diff','keep_user','--build-target','LAB006_Cordic.debug_GCC+NUCLEO-C542RC')
}
$configure=@('cmake','--preset',$preset)
if($Fresh){$configure+='--fresh'}
Invoke-Cube $configure $src
Invoke-Cube @('cmake','--build','--preset',$preset,'--parallel','8') $src
if(!(Test-Path $elf)){throw "Missing ELF: $elf"}
if($Action -eq 'Flash') {
  $listing=& cube programmer -l stlink 2>&1
  if($LASTEXITCODE -ne 0){throw 'ST-LINK enumeration failed'}
  $matchesFound=@([regex]::Matches(($listing -join "`n"),'ST-LINK SN\s*:\s*(\S+)'))
  if($matchesFound.Count -ne 1){throw 'Expected exactly one ST-LINK'}
  Invoke-Cube @('programmer','-c','port=SWD',"sn=$($matchesFound[0].Groups[1].Value)",'ap=1','mode=UR','reset=HWrst','freq=1000','-w',$elf,'-v','-rst')
}
