# LAB-000: Board and Toolchain Bring-up

## Goal

Record verified NUCLEO-C542RC/MB2213 hardware facts, tool versions, build entry points, ST-LINK connectivity, and the virtual COM port before generating application labs.

## Current status

Software tools are present and the board is connected. CubeMX 2 created and saved the STM32C542RCT6 application configuration, but no source build has been generated yet because GPIO/UART board configuration is still pending.

## Evidence log

See evidence.md. Do not mark this lab verified from software installation alone.

## Current command notes

CubeMX 2 created the LAB000_BoardBringup.ioc2 configuration for STM32C542RCT6. USART2 TX/RX are assigned to PA2/PA3 and labels USER_LED/USER_BUTTON are assigned to PA5/PC13. GPIO mode selection and CMake source generation remain pending.
