# LAB-003 验证证据

## Software

CubeMX2 generated TIM2_CH1 on PA5 and USART2 on PA2/PA3. A fresh Arm GNU 14.2.1 CMake/Ninja build passed all 26 compile/link steps. The generated initialization configures PSC=143, ARR=999, PWM1 pulse=500; the timer counter clock is 144 kHz, giving approximately 144 Hz PWM.

## Hardware

Flash was performed with STM32CubeProgrammer 2.23.0 through ST-LINK AP1, Under Reset, hardware reset, 1000 kHz. The 17.33 KiB ELF download completed with `Download verified successfully`; target identification was STM32C53x/542, device ID 0x44F, Rev Z, 3.29 V.

COM3 was opened at 115200/8-N-1, its receive buffer was cleared, and the MCU was reset. A clean capture began with `[LAB-003] TIM2 hardware PWM ready`, followed by duty reports from `0/1000` through `1000/1000` and back to `0/1000`, at 20 ms intervals.

After the reset, AP1 register reads provided independent hardware evidence: GPIOA MODER showed PA5 in alternate-function mode, GPIOA AFRL contained AF1 for PA5, TIM2 CR1 had CEN set, TIM2 CCMR1 selected PWM mode 1, ARR was `0x3E7` (999), and CCR1 was observed changing. These values establish that PA5 is driven by a running TIM2 PWM channel rather than a software-only serial demonstration.

Known CubeMX2 behavior: version 2.1.0 generated the TIM2 channel but omitted its GPIO initialization despite the pinout assignment. The project-owned compatibility function in `app/app_timer_pwm.c` supplies the documented PA5 AF1 configuration while leaving generated code untouched.
