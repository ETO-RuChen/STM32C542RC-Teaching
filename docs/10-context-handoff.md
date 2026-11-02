# Context Handoff

Current stage: repository bootstrap
Current branch: main
Last verified lab: none
Hardware state: ST-LINK Debug and STLink Virtual COM enumerated by Windows
COM port: COM3

## Completed
- Created a separate public GitHub repository for the teaching project.
- Added AI-readable project rules and metadata.
- Confirmed Windows enumeration of ST-LINK and COM3.

## Incomplete
- Confirm official board resources and STM32C542RC capability matrix.
- Create and verify LAB-000 board/toolchain bring-up.
- Add first reusable board drivers.
- Confirm MCU ID with a programmer/debug probe and verify UART mapping with a minimal firmware.

## Known issues
- No CubeMX-generated project exists here yet.
- Board silkscreen and MCU ID are not yet independently recorded.
- STM32 programmer CLI was not found in standard installation paths.

## Next Single Action
Create the first CubeMX/CMake board bring-up project and verify LED plus USART2/COM3 on hardware.
