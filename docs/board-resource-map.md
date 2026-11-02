# Board Resource Map

This is an evidence-backed working map. Items marked expected require confirmation by the board user manual or a running firmware test.

| Resource | Current value | Evidence/status |
|---|---|---|
| Board | NUCLEO-C542RC / MB2213 | CubeProgrammer board report; confirmed |
| MCU | STM32C542 family, Cortex-M33 | CubeProgrammer Device ID 0x44F; confirmed |
| Flash/NVM | 256 KB | CubeProgrammer; confirmed |
| ST-LINK | V3J17M11, probe SN recorded in lab evidence | CubeProgrammer; confirmed |
| Debug access port | AP1 | AP1 succeeds, AP2 fails; confirmed operationally |
| ST-LINK VCP | COM3 | Windows PnP; confirmed enumeration |
| UART mapping | USART2 PA2/PA3 | expected from existing board notes; firmware confirmation pending |
| LD1 LED | PA5, active high | CubeMX2 Board Part plus LAB-001 GPIO/output sampling; confirmed |
| B1 button | PC13, active high | CubeMX2 Board Part and generated binding; physical press checklist pending |
