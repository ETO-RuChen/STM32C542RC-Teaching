# LAB-004 Architecture Note

依赖为 app/main.c -> app/adc_capture.c -> generated HAL -> HAL/LL/CMSIS。应用拥有 64-word 缓冲区、统计和日志策略；采集适配层提供初始化与有超时的阻塞采集 API。中断回调只设置标志，返回前停止 DMA，应用随后读取缓冲区。

API 只允许主循环单调用者，无线程安全保证。它是实验内的适配边界，还不是可复用公共驱动。引入 RTOS/OSAL 时再适配信号量/通知，并规定取消和恢复语义。generated/vendor 文件不承担应用策略。
