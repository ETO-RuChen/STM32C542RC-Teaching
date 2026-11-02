# Platform Contract

The platform layer owns startup, linker settings, clocks, chip selection, and board resource mapping. It must not contain application policy. The NUCLEO-C542RC board contract is currently YAML-only until LAB-000 generates the first source project.

Any driver that needs a board pin or peripheral instance consumes the board contract rather than hard-coding a lab-specific global.
