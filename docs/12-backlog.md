# Living Backlog

Items stay here when they need hardware, a missing tool, or a later architectural phase.

## Hardware-dependent

- [x] Generate a complete CubeMX2 CMake/HAL project and flash the LAB-001 image.
- [x] Implement LAB-001 independently; the original experiments repository was workflow reference only.
- [x] Resolve CubeMX2 CMake export and record a reproducible generation command.
- [x] Confirm PA5 LED polarity and PC13 active level from Board Part configuration; physically press B1 once when the user returns.
- [x] Confirm USART2 PA2/PA3 to COM3 with firmware in LAB-002.
- [x] Record a clean ST-LINK build/flash/verify/debug session through AP1.

## Tooling and architecture

- [x] Add repository validation script and CI check.
- [x] Create platform/board/nucleo-c542rc contract.
- [ ] Extract LED, button, UART, and logging modules only after a second consumer proves reuse.
- [x] Add an initial CubeMX-observed capability matrix; replace pending entries with official-source and hardware evidence.

## Future labs

- [ ] Timer/PWM and advanced timer waveform labs.
- [ ] ADC/DMA and GPDMA linked-list labs.
- [ ] FDCAN loopback without external transceiver.
- [ ] AES, Hash, RNG, CORDIC, TrustZone, USB, QSPI, and FPU labs.
- [ ] Bootloader, IAP, transport-independent OTA, FreeRTOS, OSAL, and enterprise capstone.
