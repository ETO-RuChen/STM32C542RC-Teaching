# LAB-005 验证证据

## 已通过

- 全新 CubeMX2 `NUCLEO-C542RC` 空项目成功导出 CMake。
- Arm GNU Toolchain 14.2.1 配置为 Cortex-M33 + FPv5-SP hard-float。
- `LAB005_Fpu.elf` 已成功构建。
- 通过 STM32CubeProgrammer 2.23.0 使用 ST-LINK AP1 烧录并校验成功；Device ID `0x44F`，Cortex-M33，256 KiB NVM。

## 待验证

- 打开 COM3 后再执行 AP1 烧录/复位，捕获到：

  ```text
  [LAB-005] Cortex-M33 FPU benchmark ready
  [LAB-005] hard-float compiler path, USART2 115200-8-N-1
  [LAB-005] 1.25*2.5+1.25/2.5 x1000 = 3625
  ```

  计算式的数学结果为 3.625，因此乘以 1000 后的 `3625` 与预期一致。
- COM3 捕获包含复位前已经存在的乱码字节，原因是串口端在复位前已接收到旧数据；有效启动帧和计算行内容正确。
- 未接逻辑分析仪时不宣称 UART 波形已被外部仪器验证。
