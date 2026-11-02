# LAB-002 验证证据

软件：CubeMX2 1.1.2 独立生成成功；Arm GNU 14.2.1 下全新 CMake/Ninja 构建 24 步全部通过。生成 USART2 文件为 generated/hal/mx_usart2.c 和 mx_usart2.h，应用代码在 app/，未编辑生成源。

烧录：STM32CubeProgrammer 2.23.0，ST-LINK V3J17M11，AP1，Under Reset，HWrst，1000 kHz，NUCLEO-C542RC，3.29 V，Device ID 0x44F，Rev Z，Cortex-M33，256 KiB NVM，镜像 14.76 KiB；Download verified successfully，MCU reset 完成。

串口：COM3 以 115200/8-N-1 打开并在复位后采集。启动横幅为 [LAB-002] STM32C542RC USART2 logging ready，随后为 [LAB-002] 115200 8-N-1, ST-LINK VCP, PA2/PA3；seq=0 tick_ms=8、seq=1 tick_ms=1008、seq=2 tick_ms=2008、seq=3 tick_ms=3008、seq=4 tick_ms=4008。无人值守采集达到 seq=18，证明一秒周期和主循环持续运行。
