# LAB-004 Architecture Note

目标分层为 generated HAL -> ADC/DMA driver boundary -> application-owned sample buffer -> UART service -> lab app。即使生成器恢复，也不把 ADC 寄存器配置或 DMA 策略写进可复用业务层。
