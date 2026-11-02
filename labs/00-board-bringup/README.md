# LAB-000: Board and Toolchain Bring-up

## Goal

Record verified NUCLEO-C542RC/MB2213 hardware facts, tool versions, build entry points, ST-LINK connectivity, and the virtual COM port before generating application labs.

## Current status

The board/toolchain path is verified by LAB-001: CubeMX2 generated a complete CMake/HAL project, Arm GNU built it, and CubeProgrammer flashed and verified it through ST-LINK AP1. UART data-path verification remains intentionally assigned to LAB-002.

## Evidence log

See evidence.md. Do not mark this lab verified from software installation alone.

## Current command notes

LAB-001 records the reusable command path and confirms PA5 GPIO output on hardware. The LAB-000 ioc2 remains a board-discovery artifact; application work starts from an independent lab configuration.
