# Lab Index

Lab status is evidence-based: planned, in-progress, verified, or blocked. A planned lab is not a hardware claim.

| ID | Lab | Status | Dependencies |
|---|---|---|---|
| LAB-000 | Board and toolchain bring-up | verified | none |
| LAB-001 | LED and button | in-progress (build/flash/runtime passed; physical B1 check pending) | LAB-000 |
| LAB-002 | UART logging | verified | LAB-000 |
| LAB-003 | Timer and PWM | in-progress (software/register verified; external waveform pending) | LAB-001, LAB-002 |
| LAB-004 | ADC and DMA | verified (64-sample blocks via LPDMA1; COM3 capture) | LAB-002 |
| LAB-005 | Cortex-M33 FPU | verified | LAB-002 |
| LAB-006 | CORDIC Q1.31 cosine and sine | verified | LAB-002 |
| LAB-012 | Advanced timer: internal counter, RCR and software break | verified (internal only; external waveforms/deadtime pending) | LAB-002, LAB-003 |
| LAB-013 | LPDMA three-node linked-list memory transfer | verified (linear queue; circular ADC buffering pending) | LAB-002, LAB-004 |
| LAB-007 | CAN internal loopback and standard-ID filter | verified (classic CAN; external bus/FD pending) | LAB-002 |
| LAB-008 | Crypto and TrustZone | in-progress (AES-128 ECB KAT verified; HASH/RNG/TrustZone planned) | LAB-002 |
| LAB-009 | Boot, IAP, and OTA | planned | LAB-002, LAB-008 |
| LAB-010 | FreeRTOS and OSAL | planned | LAB-002, LAB-004 |
| LAB-011 | Enterprise capstone | planned | LAB-003, LAB-006, LAB-008, LAB-009, LAB-010 |
