[CmdletBinding()]
param([ValidateSet('Generate','Build','Flash')][string]$Action='Build',[switch]$Fresh)
$ErrorActionPreference='Stop'
$root=Split-Path -Parent $PSScriptRoot
$lab=Join-Path $root 'labs/05-fpu'
$src=Join-Path $lab 'LAB005_Fpu_cmake'
$preset='debug_GCC_NUCLEO-C542RC'
$elf=Join-Path $src "build/$preset/LAB005_Fpu.elf"
function Invoke-Cube([string[]]$a,[string]$wd=$root){Push-Location $wd;try{& cube @a;if($LASTEXITCODE-ne 0){throw "cube failed: $($a-join ' ')"}}finally{Pop-Location}}
function Build{$c=@('cmake','--preset',$preset);if($Fresh){$c+='--fresh'};Invoke-Cube $c $src;Invoke-Cube @('cmake','--build','--preset',$preset,'--parallel','8') $src;if(!(Test-Path $elf)){throw "Missing ELF: $elf"}}
function Serial{$x=& cube programmer -l stlink 2>&1;if($LASTEXITCODE-ne 0){throw ($x-join [Environment]::NewLine)};$m=@([regex]::Matches(($x-join [Environment]::NewLine),'ST-LINK SN\s*:\s*(\S+)'));if($m.Count-ne 1){throw 'Expected exactly one ST-LINK.'};$m[0].Groups[1].Value}
switch($Action){'Generate'{Invoke-Cube @('mx','ide-project','generate','--project-path',(Join-Path $lab 'LAB005_Fpu.ioc2'),'--format','CMake','--source','include-packs-from-local','--destination',$src,'--force','--diff','keep_user','--build-target','LAB005_Fpu.debug_GCC+NUCLEO-C542RC');Build}'Build'{Build}'Flash'{Build;$sn=Serial;Invoke-Cube @('programmer','-c','port=SWD',"sn=$sn",'ap=1','mode=UR','reset=HWrst','freq=1000','-w',$elf,'-v','-rst') $src}}
