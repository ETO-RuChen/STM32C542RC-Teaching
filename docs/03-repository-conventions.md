# Repository Conventions

One Git repository contains independent labs, reusable modules, documentation, and boot/update work. Shared code belongs under platform, drivers, middleware, services, rtos, or osal only after it meets the reuse criteria.

Layer direction: vendor -> platform -> drivers -> middleware/services -> rtos/osal -> apps/labs. Extract code only with a stable interface, documented dependencies, a minimal example, and an automated test or recorded board verification.
