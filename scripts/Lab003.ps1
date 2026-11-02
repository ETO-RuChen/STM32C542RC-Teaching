[CmdletBinding()]
param([ValidateSet('Generate','Build','Flash')][string]$Action='Build',[switch]$Fresh)
$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
$lab=Join-Path $root 'labs/03-timer-pwm'
$ioc=Join-Path $lab 'LAB003_TimerPwm.ioc2'
$src=Join-Path $lab 'LAB003_TimerPwm_cmake'
$preset='debug_GCC_NUCLEO-C542RC'
$elf=Join-Path $src "build/$preset/LAB003_TimerPwm.elf"
function Invoke-Cube([string[]]$a,[string]$wd=$root){Push-Location $wd;try{& cube @a;if($LASTEXITCODE-ne 0){throw "cube failed: $($a-join ' ')"}}finally{Pop-Location}}
function Generate{Invoke-Cube @('mx','ide-project','generate','--project-path',$ioc,'--format','CMake','--source','include-packs-from-local','--destination',$src,'--force','--diff','keep_user','--build-target','LAB003_TimerPwm.debug_GCC+NUCLEO-C542RC')}
function Build{$c=@('cmake','--preset',$preset);if($Fresh){$c+='--fresh'};Invoke-Cube $c $src;Invoke-Cube @('cmake','--build','--preset',$preset,'--parallel','8') $src;if(!(Test-Path $elf)){throw "Missing ELF: $elf"}}
function Serial{$x=& cube programmer -l stlink 2>&1;if($LASTEXITCODE-ne 0){throw ($x-join [Environment]::NewLine)};$m=@([regex]::Matches(($x-join [Environment]::NewLine),'ST-LINK SN\s*:\s*(\S+)'));if($m.Count-ne 1){throw 'Expected exactly one ST-LINK.'};$m[0].Groups[1].Value}
switch($Action){'Generate'{Generate;Build}'Build'{Build}'Flash'{Build;$sn=Serial;Invoke-Cube @('programmer','-c','port=SWD',"sn=$sn",'ap=1','mode=UR','reset=HWrst','freq=1000','-w',$elf,'-v','-rst') $src}}
