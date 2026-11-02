# LAB-005：Cortex-M33 单精度 FPU

本实验使用 STM32C542 的 Cortex-M33 单精度浮点单元，比较并理解编译器的 `-mfpu=fpv5-sp-d16 -mfloat-abi=hard` 路径。应用层通过 USART2/ST-LINK VCP 输出一个可重复的浮点表达式结果，不需要外接设备。

## 操作

```powershell
.\scripts\Lab005.ps1 -Action Generate -Fresh
.\scripts\Lab005.ps1 -Action Flash
```

串口参数：115200-8-N-1，预期输出 `1.25*2.5+1.25/2.5 x1000 = 3437`。本机本轮已完成 AP1 烧录/校验，但尚未捕获 COM3 文本。

生成目录中的 vendor/DFP 文件由 CubeMX2 管理；`app/` 与顶层 CMake 的 FPU 应用接线是项目自有代码。
