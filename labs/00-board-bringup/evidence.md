# LAB-000 Evidence

Date: 2026-11-03 (Asia/Shanghai)

## Software discovery

- STM32Cube wrapper: 0.11.0
- CMake: 4.4.4
- Ninja: 1.13.2
- Arm GNU Toolchain: 14.2.1
- VS Code: 1.144.0

## Hardware checks

- Board identity: NUCLEO-C542RC is the working target; physical silkscreen confirmation still pending
- ST-LINK connection: Windows enumerated ST-Link Debug (USB VID_0483/PID_3754)
- MCU ID: pending; no programmer CLI was found in the standard ST installation paths
- Virtual COM port: STMicroelectronics STLink Virtual COM Port (COM3)
- UART mapping: expected USART2 PA2/PA3 from the existing board notes; must be confirmed against this board and firmware

## Next test

Connect the board with a data-capable USB cable and run the non-destructive ST-LINK/COM discovery step.
