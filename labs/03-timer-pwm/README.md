# LAB-003：TIM2 硬件 PWM

本实验把 LAB-001 的软件 PWM 替换为定时器输出比较单元的硬件 PWM。TIM2_CH1 复用到板载 LED PA5，不需要接线；USART2/COM3 继续输出测量辅助信息。

- TIM2_CH1：PA5
- PWM1，计数频率 144 kHz，ARR=999，PSC=143
- 输出频率约 144 Hz
- USART2：PA2/PA3，115200-8-N-1，COM3
- 烧录：ST-LINK AP1

运行 scripts/Lab003.ps1 -Action Generate -Fresh 生成并构建，运行 scripts/Lab003.ps1 -Action Flash 烧录。COM3 会输出占空比 duty=0..1000，示波器探头可直接观察 PA5 的约 144 Hz 波形和逐步变化的高电平时间。

生成代码在 LAB003_TimerPwm_cmake/generated，应用代码在 app。应用只调用生成的 TIM2/USART2 handle 和 HAL 接口，不修改 CubeMX2 生成文件。
