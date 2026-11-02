# LAB-002 架构说明

依赖方向为 generated HAL -> app_uart_logging -> app_entry -> main。生成代码负责时钟、GPIO 复用和 USART2 初始化；应用层负责日志内容、周期策略和失败处理。没有把 printf 重定向到全局 libc，因为显式调用 HAL_UART_Transmit 更容易在调试器和后续 DMA 实验中追踪。

post_system_init_hook() 在 USART2 初始化完成后调用，因此启动横幅不会访问未初始化的 UART。发送超时固定为 1000 ms；此版本不在中断中发送，也不启用 DMA。
