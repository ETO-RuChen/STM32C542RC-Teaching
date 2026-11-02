# Living Backlog

Items stay here when they need hardware, a missing tool, or a later architectural phase.

## Hardware-dependent

- [ ] Generate LAB-000 CMake source and flash a minimal LED/UART image.
- [ ] Implement LAB-001 independently; use the original experiments repository as reference only.
- [ ] Resolve CubeMX 2 CMake export destination/generation behavior and capture the generated project.
- [ ] Confirm LED polarity, button active level, and USART2 to COM3 using firmware.
- [ ] Record a clean ST-LINK debug session through AP1.

## Tooling and architecture

- [ ] Add repository validation script and CI check.
- [ ] Create platform/board/nucleo-c542rc contract.
- [ ] Extract LED, button, UART, and logging modules only after evidence.
- [x] Add an initial CubeMX-observed capability matrix; replace pending entries with official-source and hardware evidence.

## Future labs

- [ ] Timer/PWM and advanced timer waveform labs.
- [ ] ADC/DMA and GPDMA linked-list labs.
- [ ] FDCAN loopback without external transceiver.
- [ ] AES, Hash, RNG, CORDIC, TrustZone, USB, QSPI, and FPU labs.
- [ ] Bootloader, IAP, transport-independent OTA, FreeRTOS, OSAL, and enterprise capstone.
