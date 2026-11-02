# LAB-004：ADC 内部 VrefInt + DMA

本实验目标是在不接外部模拟设备的情况下，用 ADC1 的内部 VrefInt 通道配合 DMA 连续采样，并通过 USART2/ST-LINK VCP 输出样本。

当前状态是 `in-progress`：CubeMX2 CLI 已成功创建工程，并成功写入 ADC1、VrefInt、软件触发、规则序列和 DMA 参数；但 STM32CubeMX2 1.1.2 的 HAL ADC 代码生成器在导出时稳定报：

```text
Component "ADC" : TypeError: Cannot convert undefined or null to object
```

复现命令：

```powershell
cube mx ide-project generate --project-path labs/04-adc-dma/LAB004_AdcDma.ioc2 --format CMake --source include-packs-from-local --destination labs/04-adc-dma/LAB004_AdcDma_cmake --force --diff keep_user --build-target LAB004_AdcDma.debug_GCC+NUCLEO-C542RC
```

在生成器修复或找到官方兼容配置前，不把本实验标记为可构建、可烧录或已验证。源配置保留在 `LAB004_AdcDma.ioc2`，便于后续升级 CubeMX2 后直接重试。
