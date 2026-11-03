# LAB-012：高级定时器第一课——内部计数、RCR 与软件刹车

本课用 TIM1 演示中心对齐计数、重复计数器和主输出门控，已通过板上串口自检。只需 NUCLEO-C542RC 和 ST-LINK USB；前置 LAB-002、LAB-003。未映射 TIM1 到 GPIO，因此不需接线，板载 LED 不作为本课输出。

## 从配置到工程

1. CubeMX2 选择 NUCLEO-C542RC，创建独立 LAB012_AdvancedTimer 空工程，启用 HALCore、SystemInit。
2. 启用 TIM1 内部时钟，prescaler=47，period=1000，counter mode=CENTER_UP_DOWN，repetition counter=3。
3. 启用 Channel 1，比较模式 PWM1，pulse=250，使能 OC 通道。保持外部 GPIO 未映射；当前配置只运行内部比较逻辑。
4. 启用 break function 的输出级配置；保持自动恢复 AOE 关闭，不启用外部刹车源。应用通过 BG 软件事件触发刹车。
5. 启用 USART2 Async（PA2/PA3），115200-8N1。导出完整 CMake/HAL/CMSIS/DFP，顶层 CMake 接入自有 app、DFP system/startup 及 Flash 链接脚本。
6. 编译、烧录，通过 ST-LINK VCP 查看自检。

从仓库根目录运行：

```powershell
.\scripts\Lab012.ps1 -Action Build -Fresh
.\scripts\Lab012.ps1 -Action Flash
.\scripts\Lab012.ps1 -Action Generate -Fresh
```

Generate 在 out 下全新导出后合并，保留自有 app 和顶层 CMake；删除组件时仍需检查旧文件。VS Code 可打开 LAB012_AdvancedTimer_cmake，使用 debug_GCC_NUCLEO-C542RC preset。调试连接选择 AP1。

## 为什么 RCR 不等于 PWM 分频器

本工程 TIM1 输入时钟为 48 MHz，PSC=47，所以计数时钟为 1 MHz。中心对齐从 0 到 ARR 再回到 0，完整计数周期为 2×1000 个计数时钟，内部 PWM 周期理论为 2 ms，即 500 Hz。CCR1=250 对应约 25% 的内部比较占空比。

计数到顶和到底各产生一次更新机会，因此 RCR=0 时更新事件约 1000 次/秒。RCR=3 每四次更新机会才放行一次，更新事件约 250 次/秒，PWM 本身仍约 500 Hz。程序分别测量 100 ms 内的更新标志，实测 100 和 25，同时观察 DIR 的两个状态。

CubeMX2 生成 mx_tim1.c 中的“999 Hz”注释未体现此中心对齐周期；本课根据 PSC/ARR/CMS 和板上更新计数解释行为，未改写生成代码。串口与 SysTick 检查是粗粒度功能验证，不是外部精确频率测量。

## 刹车与恢复

CH1 通道使能和 BDTR.MOE 主输出使能是不同层次。探针先使能 MOE，再产生 BG 软件刹车，等待硬件反馈 MOE=0，检查 BIF；保持计数器运行 20 ms，验证 AOE=0 时仍保持关闭，然后清 BIF 并显式恢复 MOE。每轮结束再次关闭输出与计数器。

```text
[LAB-012] cfg=1 dirs=3 updates_100ms: RCR0=100 RCR3=25 MOE=1->0 BIF=1 held=1 rearm=1 PASS
```

写寄存器后紧接着读回可能尚未反映反馈状态。本课使用最多 5 ms 的有界等待确认 MOE 变化；它不是刹车延迟测量。AOE 关闭时，清 BIF 也不等于允许输出恢复。这里的自动恢复测试只针对内部软件事件，不是外部故障保护认证。

## 分层、调试和练习

generated/hal 管理初始配置；app/timer_probe.c 用 LL 执行可观察的诊断操作，返回结果结构；main.c 负责验收与日志。探针改变 RCR 为 0 和 3，所以不同时通过 HAL 启动同一 TIM1。它是实验内单调用者诊断适配，尚未提取公共驱动。

可观察 CR1.CMS/DIR、PSC、ARR、RCR、CCR1、SR.UIF/BIF、BDTR.MOE/AOE。调试暂停会改变计时结果，不将单步下的频率统计作为正常结果。练习将 RCR=3 改成 1，推导更新数约 50/100 ms，并同步修改验收；再改变 ARR 并推导完整周期与更新频率。

## 限制与下一课

- 当前验证内部计数、比较配置与输出门控；未验证 GPIO 波形、互补输出、死区时间或真实 BKIN 输入。
- 外部波形课再映射一对允许的 TIM1_CH1/CH1N 引脚，用逻辑分析仪或示波器检查互补与死区；不需要电机。不能只凭 DTG 寄存器值宣称死区波形已通过。
- 没有串口输出时先检查 COM 端口占用、115200-8N1、AP1 烧录；计数失败时检查 TIM1 输入时钟、CMS、PSC/ARR、RCR 和是否暂停了目标。
- snprintf 造成 nosys 文件 I/O 链接警告；应用通过 HAL_UART_Transmit 发送日志，不提供文件系统。
- 完整 vendor/generated 导入含供应商既有尾部空格，自有源代码和文档单独执行空白检查。

参考：[ST STM32C5 LL TIM API](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.0.0/en/docs/drivers/ll_drivers/tim/api/ll_tim_exported_functions.html)。本工程具体实现以签入的 HAL/LL 2.1.0 头文件为准，可查 LL_TIM_SetRepetitionCounter、LL_TIM_EnableAllOutputs、LL_TIM_GenerateEvent_BRK。Git 提交记录见 `git log -- labs/12-advanced-timer`。
