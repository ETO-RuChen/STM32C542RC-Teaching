# Context Handoff

Current stage: LAB-002 verified; LAB-001 physical B1 observation remains pending
Current branch: main
Last verified lab: LAB-002; LAB-001 build/flash/runtime evidence passed with physical B1 checklist pending
Hardware state: LAB-002 built, flashed, verified and running USART2 logging through AP1
COM port: COM3

## Completed
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

## Incomplete
- Physically press B1 once and visually confirm exactly one LAB-001 mode switch.
- Continue replacing CubeMX-observed capability entries with official references and hardware evidence.
- Keep LAB-001 BSP local until a second lab proves the interface is reusable.

## Known issues
- CubeMX2 1.1.2 exports `mx_button.c` without `mx_button.h`; `platform/cubemx2_compat` contains the documented workaround.
- AP0/AP2 do not identify the target in this setup; AP1 is required.
- LAB-002 uses blocking UART transmit intentionally; DMA UART is a later lab.
- No one was available to press B1 during unattended verification, so physical debounce/edge behavior remains an explicit manual check.

## Next Single Action
Create LAB-003 as an independent timer/PWM project, while keeping LAB-001 B1 observation as a manual backlog item.
