---
name: stm32-project
description: Operate this STM32C542RC teaching repository using its AI-readable rules, metadata, Git workflow, and verification gates.
---
Read AGENTS.md, project metadata, the context handoff, and git status before changing files. Work on one verifiable goal. Do not claim hardware support without evidence. After changes, run narrow checks, update documentation and handoff state, review the diff, and report the required completion fields.

When creating an empty CubeMX2 HAL peripheral project, inspect global-services and enable HALCore and SystemInit before export. LAB-006 reproduced a null-object codegen failure without them and exported successfully after enabling both. Do not diagnose that error as a vendor defect before checking these dependencies. Keep the project-owned entry point and startup/linker integration explicit.

After configuration changes, compare exported source with the ioc2. LAB-004 observed successful in-place export retaining old files, even on a retry with overwrite. Use a fresh export destination and merge only after inspecting generated initialization; scripts/Lab004.ps1 demonstrates this. Query enumerated parameter values instead of guessing display strings: ADC sequence VREFINT differs from the channel-list key VrefInt. Do not equate successful export or compilation with correct clock setup or working hardware.
