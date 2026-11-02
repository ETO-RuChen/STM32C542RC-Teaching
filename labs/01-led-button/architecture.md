# LAB-001 Architecture Note

## Dependency rule

The application depends on BSP interfaces; BSP depends on generated HAL pin aliases. Generated and vendor code never depends on project policy.

| Layer | Owns | Must not own |
|---|---|---|
| generated/vendor | clock, cache, HAL and GPIO initialization | breathing/blink policy |
| platform compatibility | a narrow CubeMX2 export workaround | application behavior |
| BSP | physical LED writes and debounced button events | mode timing |
| app | mode state machine, timing and PWM policy | board register addresses |

## Initialization sequence

```text
reset -> startup -> main -> mx_system_init
                         -> HAL/clock/cache/GPIO
                         -> post_system_init_hook
                         -> app_led_modes_run (non-returning)
```

`post_system_init_hook` is a generated-code extension point. A project-owned strong definition overrides the generated weak default, so no generated C source needs hand editing.

## Reuse decision

The LED and button modules remain local to LAB-001 until another lab consumes the same interface and hardware evidence exists. At that point they may move to the repository-level `platform/` or `drivers/` tree. This prevents premature abstraction.
