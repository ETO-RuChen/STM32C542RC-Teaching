# Context Handoff

Current stage: LAB-007 classic CAN internal loopback/filter verified; LAB-012 internal probe verified; external waveform/BKIN and LAB-001 physical B1 remain pending
Current branch: main
Last completed lab evidence: LAB-007 five consecutive COM3 rounds passed: accepted 0x123 plus payload, rejected 0x321 after successful internal transmission
Hardware state: LAB-007 flashed through AP1; FDCAN1 internal loopback classic CAN 500 kbps; PA11/PA12 assigned AF9, target USB not used
COM port: COM3

## Completed
- Added LAB-007 full CubeMX2 project, internal classic CAN loopback/filter adapter, tutorial/evidence, and Lab007.ps1. Configured nominal 500 kbps, standard filter 0x123/0x7FF; accepted payload and negative-filter checks passed.
- Added LAB-012 full CubeMX2 CMake project, internal TIM1 center-aligned/RCR/software-break probe, tutorial, evidence, and Lab012.ps1. Hardware feedback requires bounded polling for MOE changes; immediate reads observed stale state.
- Resolved LAB-004 export by enabling HALCore/SystemInit; selected official sequence value VREFINT and configured RCC HCLK/2 (24 MHz) ADC clock. Added complete generated/vendor code, bounded block capture adapter, application-owned sample buffer, and teaching documents.
- Diagnosed stale in-place export; fresh destination produced matching clock/system sources. Lab004.ps1 regenerates through a fresh out directory, preserves app/top-level integration, and builds the result.
- Resolved LAB-006 null-object export failure by enabling HALCore and SystemInit in the empty project; documented the differential result and taught the repository skill this prerequisite.
- Added full LAB-006 CORDIC CMake project, Q1.31 teaching material, four-vector selftest, and Lab006.ps1. HAL 2.1.0 single-vector polling loop boundary is documented and avoided through batch input.
- Created a separate public GitHub repository for the teaching project.
- Added AI-readable project rules and metadata.
- Confirmed Windows enumeration of ST-LINK and COM3.
- Confirmed read-only CubeProgrammer connection through AP1: Device ID 0x44F, Rev Z, 256 KB NVM.
- Created and saved the first STM32CubeMX 2 ioc2 configuration for LAB-000.
- Assigned USART2 PA2/PA3 and labeled PA5/PC13 for the board LED/button in the ioc2.
- Added a living backlog, architecture roadmap, repository validation script, and GitHub Actions structure check.
- Added a CubeMX-observed capability matrix and NUCLEO-C542RC platform contract.
- CubeMX resolved STM32C542RCT6 with SP_FPU/DSP and default TrustZone-disabled attributes.
- Created an independent full CubeMX2 Board/HAL/CMake project under `labs/01-led-button`.
- Configured PA5 output and PC13 input in the ioc2; generated `mx_gpio_default.c/.h`.
- Added project-owned platform/BSP/app layers without editing generated C sources.
- Worked around CubeMX2 1.1.2 omitting `mx_button.h` with a narrow compatibility header.
- Built `LAB001_LedButton.elf` with Arm GNU 14.2.1.
- Flashed and verified 7.67 KiB through ST-LINK AP1; Device ID 0x44F, Rev Z, 3.29-3.30 V.
- Hardware debug sampling confirmed default PA5 `1 -> 0 -> 1` timing and a running system tick.
- Debug state injection confirmed independent breathing level/PWM progression; reset restored default blink mode.
- Added `scripts/Lab001.ps1` for reproducible generate/build/flash operations.
- Created and verified LAB-002 USART2/ST-LINK VCP logging; clean COM3 capture confirmed the one-second sequence.
- Created and verified LAB-003 TIM2_CH1 hardware PWM on PA5; AP1 flash verification, clean COM3 duty sweep, and GPIO/TIM2 register evidence passed.
- Recorded the CubeMX2 2.1.0 omission of TIM2 GPIO setup and isolated the project-owned AF1 compatibility initialization in `app/app_timer_pwm.c`.
- Added LAB-004 ADC/VrefInt/DMA source configuration and documented the reproducible CubeMX2 ADC generator null-object blocker.
- Added LAB-005 Cortex-M33 single-precision FPU project with hard-float flags, self-contained CMSIS core headers, project-owned UART/app layer, build script, and teaching documents.
- Built LAB-005 successfully and flashed/verified it through ST-LINK AP1 (Device ID 0x44F, Rev Z, 256 KiB).
- Captured LAB-005 runtime output on COM3 after opening the port before reset; the hard-float expression reported `3625`, matching 3.625 × 1000.

## Incomplete
- Physically press B1 once and visually confirm exactly one LAB-001 mode switch.
- Continue replacing CubeMX-observed capability entries with official references and hardware evidence.
- Keep LAB-001 BSP local until a second lab proves the interface is reusable.

## Known issues
- CubeMX2 1.1.2 exports `mx_button.c` without `mx_button.h`; `platform/cubemx2_compat` contains the documented workaround.
- CubeMX2 2.1.0 may omit GPIO initialization for a TIM output channel; LAB-003 documents the narrow compatibility layer and leaves generated files untouched.
- AP0/AP2 do not identify the target in this setup; AP1 is required.
- LAB-002 uses blocking UART transmit intentionally; DMA UART is a later lab.
- No one was available to press B1 during unattended verification, so physical debounce/edge behavior remains an explicit manual check.
- CubeMX2 1.1.2 also reports the same null-object generator failure for minimal RNG/UART configurations; the exploratory RNG source was not retained as a lab.
- LAB-005's capture begins with stale pre-reset bytes when COM3 is opened before a running target is reset; the subsequent startup and result lines are correct.

## Next Single Action
Proceed to LAB-013 DMA linked-list memory transfer without external wiring. Keep CAN FD/BRS, external CAN bus, LAB-012 waveform/deadtime/BKIN and earlier physical checks pending.
