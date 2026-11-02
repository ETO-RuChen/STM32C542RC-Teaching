# LAB-001 Verification Evidence

Date: 2026-11-03 (Asia/Shanghai)

## Configuration source

- Board: NUCLEO-C542RC / STM32C542RCT6
- CubeMX2 project: `LAB001_LedButton.ioc2`
- PA5: GPIO output, push-pull, low speed, no pull, initial reset, active high
- PC13: GPIO input, no pull, polling
- CubeMX2 export: CMake, local packs, `keep_user` collision policy

## Reproducible build

Command:

```powershell
.\scripts\Lab001.ps1 -Action Generate -Fresh
```

Result: CubeMX2 generation succeeded and a fresh 26-step Arm GNU build linked `LAB001_LedButton.elf`. Regeneration preserved the project-owned `platform/`, `bsp/`, `app/`, and CMake additions.

Final size:

```text
text  7760
data    96
bss   1928
dec   9784
```

ELF SHA-256 at verification time:

```text
A81E2F1935B5E9EDE3968D6684B4198FEBE346F3FFCFE06408FA0022B8AE3216
```

The ELF and build directory are ignored and are not committed; the hash is release evidence for the locally tested artifact.

## Flash and target evidence

Command:

```powershell
.\scripts\Lab001.ps1 -Action Flash
```

Observed result:

- STM32CubeProgrammer 2.23.0
- ST-LINK V3J17M11
- Board NUCLEO-C542RC
- Target voltage 3.29-3.30 V
- SWD AP1, 1000 kHz, under-reset connection
- Device ID `0x44F`, Rev Z, Cortex-M33, 256 KiB NVM
- Programmed image 7.67 KiB at `0x08000000`
- Download verified successfully
- MCU reset successfully

No option bytes, TrustZone state, read-protection level, or debug authentication setting was changed.

## Runtime sampling

### Default blink

Three hot-plug samples about 1.1 seconds apart read the PA5 output data bit as:

```text
1 -> 0 -> 1
```

The corresponding `uwTick` values increased on every sample. This confirms the target was running and the default state machine alternated PA5 with the intended one-second half-period.

### Breathe branch

The mode byte in RAM was temporarily changed from blink to breathe using the debugger, which is equivalent to the debounced press event after the mode assignment. Runtime samples showed the breathe level and PWM phase continuously changing in the breathe branch. Representative decoded levels included:

```text
89 descending -> 10 ascending -> 96 descending -> 2 descending
```

The board was then reset. A final read confirmed `mode=0` (default blink) and a running tick. The temporary RAM change did not alter Flash.

## Remaining human observation

No person was available to press B1 during unattended execution. The following remains an explicit manual observation, not a claimed automated result:

- one physical B1 press switches exactly once;
- a second press returns to blink;
- the visible breathing curve is smooth to the observer.

The input configuration, 20 ms debouncer, one-shot pressed-edge logic, exclusive mode state machine, and both output branches were reviewed and built. This narrows the remaining check to the physical switch and visual perception.
