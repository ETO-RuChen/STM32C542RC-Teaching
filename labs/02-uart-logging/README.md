# LAB-002：USART2 / ST-LINK VCP 日志

目标：使用板载 ST-LINK VCP 建立第一个可观察的软件诊断通道。

- USART2 TX/RX：PA2 / PA3
- 当前电脑端口：COM3
- 参数：115200-8-N-1，无硬件流控
- 不需要外接线；烧录仍使用 ST-LINK AP1

启动后每秒输出一行 [LAB-002] seq=... tick_ms=...。在仓库根目录运行 scripts/Lab002.ps1 -Action Generate -Fresh 和 scripts/Lab002.ps1 -Action Flash，串口终端打开 COM3 后复位板子。

CubeMX2 生成物位于 LAB002_UartLogging_cmake/generated，应用代码位于 app；本实验使用阻塞发送，为后续 DMA、FreeRTOS、IAP 和 OTA 保留清晰边界。
