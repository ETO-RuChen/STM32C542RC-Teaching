# Context Handoff

Current stage: repository bootstrap
Current branch: main
Last verified lab: none
Hardware state: ST-LINK connected; CubeProgrammer identified NUCLEO-C542RC target through AP1
COM port: COM3

## Completed
- Created a separate public GitHub repository for the teaching project.
- Added AI-readable project rules and metadata.
- Confirmed Windows enumeration of ST-LINK and COM3.
- Confirmed read-only CubeProgrammer connection through AP1: Device ID 0x44F, Rev Z, 256 KB NVM.
- Created and saved the first STM32CubeMX 2 ioc2 configuration for LAB-000.
- Assigned USART2 PA2/PA3 and labeled PA5/PC13 for the board LED/button in the ioc2.

## Incomplete
- Confirm official board resources and STM32C542RC capability matrix.
- Create and verify LAB-000 board/toolchain bring-up.
- Add first reusable board drivers.
- Confirm MCU ID with a programmer/debug probe and verify UART mapping with a minimal firmware.

## Known issues
- No CubeMX-generated project exists here yet.
- AP2 cannot identify the target; AP1 is the currently verified access port.
- UART behavior and board LED/button response still need a generated firmware test.
- CubeMX GPIO mode selection and CMake source generation are still pending.

## Next Single Action
Create the first CubeMX/CMake board bring-up project and verify LED plus USART2/COM3 on hardware, using AP1 for debug access.
