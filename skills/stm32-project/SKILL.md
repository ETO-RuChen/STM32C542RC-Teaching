---
name: stm32-project
description: Operate this STM32C542RC teaching repository using its AI-readable rules, metadata, Git workflow, and verification gates.
---
Read AGENTS.md, project metadata, the context handoff, and git status before changing files. Work on one verifiable goal. Do not claim hardware support without evidence. After changes, run narrow checks, update documentation and handoff state, review the diff, and report the required completion fields.

When creating an empty CubeMX2 HAL peripheral project, inspect global-services and enable HALCore and SystemInit before export. LAB-006 reproduced a null-object codegen failure without them and exported successfully after enabling both. Do not diagnose that error as a vendor defect before checking these dependencies. Keep the project-owned entry point and startup/linker integration explicit.
