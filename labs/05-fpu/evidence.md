# LAB-005 验证证据

## 已通过

- 全新 CubeMX2 `NUCLEO-C542RC` 空项目成功导出 CMake。
- Arm GNU Toolchain 14.2.1 配置为 Cortex-M33 + FPv5-SP hard-float。
- `LAB005_Fpu.elf` 已成功构建。
- 通过 STM32CubeProgrammer 2.23.0 使用 ST-LINK AP1 烧录并校验成功；Device ID `0x44F`，Cortex-M33，256 KiB NVM。

## 待验证

- 本轮未捕获到 COM3 文本输出，不能据此宣称串口运行已经通过；实验保持 `in-progress`。
- 未接逻辑分析仪时不宣称 UART 波形已被外部仪器验证。
