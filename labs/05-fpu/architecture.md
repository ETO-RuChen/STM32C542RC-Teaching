# LAB-005 架构

`app/app_main.c` 只承担实验策略；`app/platform_uart.c` 是本实验临时的平台适配层；CubeMX2 导出的 DFP、启动文件和链接脚本属于 vendor/platform 基础。后续第二个串口消费者出现后，再把 UART 提升为共享 driver/service。
