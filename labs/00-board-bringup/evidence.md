# LAB-000 Evidence

Date: 2026-11-03 (Asia/Shanghai)

## Software discovery

- STM32Cube wrapper: 0.11.0
- CMake: 4.4.4
- Ninja: 1.13.2
- Arm GNU Toolchain: 14.2.1
- VS Code: 1.144.0

## Hardware checks

- Board identity: CubeProgrammer reports NUCLEO-C542RC
- ST-LINK connection: Windows enumerated ST-Link Debug (USB VID_0483/PID_3754); ST-LINK probe 002E00233235510F37333439, firmware V3J17M11
- MCU ID: AP1 connection succeeded; Device ID 0x44F, Rev Z, STM32C53x/542, Cortex-M33, NVM 256 KBytes
- Virtual COM port: STMicroelectronics STLink Virtual COM Port (COM3)
- UART mapping: expected USART2 PA2/PA3 from the existing board notes; must be confirmed with firmware

## CubeMX project generation

- CubeMX 2 CLI created LAB000_BoardBringup.ioc2 for STM32C542RCT6.
- CubeMX project save returned success.
- Generated application source/build project: pending GPIO/UART configuration and code generation.
- Pinout configuration: USART2_TX=PA2 and USART2_RX=PA3 assigned; PA5 labeled USER_LED; PC13 labeled USER_BUTTON.
- GPIO mode configuration and generated CMake source: pending.

## Debug access note

AP2 returned No STM32 target found; AP1 successfully identified the target. Future programmer commands for this board should use AP1 unless official documentation proves otherwise. No erase, write, option-byte, or security operation was performed.

## Next test

Connect the board with a data-capable USB cable and run the non-destructive ST-LINK/COM discovery step.
