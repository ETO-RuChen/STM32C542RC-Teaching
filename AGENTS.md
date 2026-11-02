# STM32C542RC Teaching Repository Rules

Read this file before changing files.

- Work on one small, verifiable goal at a time.
- Inspect git status, project metadata, the relevant lab contract, and the context handoff before implementation.
- Treat chip and board capabilities as unverified until supported by official documentation, CubeMX, or a recorded hardware test.
- Keep vendor/generated code separate from project-owned code.
- Keep dependency direction toward lower layers: vendor -> platform -> drivers -> services -> RTOS/OSAL -> apps/labs.
- Do not put application policy in reusable drivers.
- Do not commit secrets, tokens, private keys, or access-bearing URLs.
- Complete only after the appropriate build/test or a documented hardware blocker.

Required completion report: Goal, Changed, Verified, Hardware Result, Known Issues, Git Commit, Next Single Action.
