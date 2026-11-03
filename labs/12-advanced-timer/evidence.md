# LAB-012 验证证据（2026-11-03）

- 官方 CubeMX2 从 NUCLEO-C542RC 新建独立空工程，启用 HALCore/SystemInit、TIM1、USART2。CMake 导出成功。
- HAL pack 2.1.0 生成 TIM1 中心对齐、PSC=47、ARR=1000、RCR=3、PWM1、CCR1=250。输出 GPIO 未映射；生成文件明确说明 TIM1 不需 GPIO。
- CMake/Ninja、Arm GNU 14.2.1 编译通过，未修改 generated/vendor 源文件。
- AP1、SWD 1000 kHz、under reset、HWrst，11.13 KB 烧录并校验成功；0x44F、Rev Z、256 KB、3.30 V。
- COM3 115200-8N1，复位后连续四轮相同结果：

```text
[LAB-012] cfg=1 dirs=3 updates_100ms: RCR0=100 RCR3=25 MOE=1->0 BIF=1 held=1 rearm=1 PASS
```

初版立即读回 MOE 时读到旧状态，报告 MOE=1->1、held=1、rearm=0。改为有界轮询等待 MOE 状态反馈后通过。此对照确认程序必须等待可观察状态；没有测量硬件刹车传播时间。

cfg 检查定时器能力、PSC/ARR/CCR/CMS、PWM 比较模式；dirs=3 表示窗口内观察到上数和下数；RCR 更新数的允许区间分别为 95..105 与 23..26。MOE/BIF/held/rearm 验证软件 break 和手动恢复路径。未作物理引脚、死区、互补波形或外部 BKIN 的验证。

随后实际执行 Lab012.ps1 Generate -Fresh：全新目录导出及全部 16 个构建步骤成功；再运行脚本 Flash，烧录校验成功，完整启动行后又采集到四轮 PASS。仓库结构检查通过（8 个实验契约）。自有文件单独执行 diff --check，供应商原有空白与 nosys 链接警告按 README 保留。
