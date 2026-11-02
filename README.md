# STM32C542RC Teaching Lab

AI-readable, reproducible teaching repository for NUCLEO-C542RC (MB2213), using STM32CubeMX 2.x, VS Code, CMake, and ST-LINK. The roadmap covers GPIO, UART, timers, DMA, CAN, QSPI, USB, cryptography, TrustZone, FPU/CORDIC, low power, FreeRTOS, OSAL, IAP, and OTA.

Start with AGENTS.md, .codex/project.yaml, and docs/10-context-handoff.md. Work on one small target, verify it, document it, and commit it. This repository is separate from STM32C542RC_Experiments.

## Current lab

[LAB-001: board LED breathe/blink with button mode switching](labs/01-led-button/README.md) is the first complete CubeMX2/HAL/CMake project. Its current source has been freshly regenerated, built, flashed and verified through AP1; blink timing and breathe-state runtime checks pass. A physical B1 press remains an explicit manual observation rather than an unverified claim. It uses only the onboard PA5 LED and PC13 button.

```powershell
.\scripts\Lab001.ps1 -Action Build
.\scripts\Lab001.ps1 -Action Flash
```

The repository intentionally keeps CubeMX2/vendor output beside project-owned `platform`, `bsp`, and `app` layers so regeneration and manual development remain distinguishable.
