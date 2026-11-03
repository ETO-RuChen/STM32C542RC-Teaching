# LAB-006 验证证据（2026-11-03）

## 生成异常的对照

原始配置由 create-from-board --empty-project 创建，启用 CORDIC 和 USART2 后，导出 CMake 报 `Component "CORDIC" : TypeError: Cannot convert undefined or null to object`，UART 和 GPDSC 也失败。工具为 CubeMX2 安装包 1.1.2、backend 1.2.0、CodeGen 9.4.5、HAL pack 2.1.0。

`cube mx global-services list -p labs/06-cordic/LAB006_Cordic.ioc2` 显示 HALCore 与 SystemInit 均为 AVAILABLE。依次执行 global-services enable HALCore 和 global-services enable SystemInit 后，同一项目、同一外设配置、同一 CMake 导出命令返回 success。生成了 mx_cordic.c、mx_usart2.c、mx_system.c 及 HAL 配置头。

此对照定位了本项目的公共服务配置遗漏；尚未重试 ADC/RNG，不能将它们的旧失败直接宣称为已修复。

## 构建与硬件

- CMake/Ninja + Arm GNU 14.2.1 构建通过。保留完整 DFP、HAL、CMSIS 和许可证；自有入口在 app/main.c。
- ST-LINK AP1、SWD 1000 kHz、under reset、HWrst 烧录 9.59 KB，并返回 Download verified successfully。
- 目标：Device ID 0x44F、Rev Z、256 KB NVM、3.30 V。
- 在烧录前打开 COM3 115200-8N1，复位后采集到以下结果，连续三轮 SELFTEST PASS 4/4：

```text
[LAB-006] CORDIC Q1.31 cosine+sine, modulus=0.5
[LAB-006] angle=0 cos_q31=1073742080 sin_q31=1024 status=3941264106 PASS
[LAB-006] angle=pi/4 cos_q31=759250944 sin_q31=759249408 status=3941264106 PASS
[LAB-006] angle=pi/2 cos_q31=0 sin_q31=1073742336 status=3941264106 PASS
[LAB-006] angle=-pi/2 cos_q31=0 sin_q31=-1073742336 status=3941264106 PASS
[LAB-006] SELFTEST PASS 4/4
```

status 是 HAL 状态枚举的原始整数；程序直接比较 status == HAL_OK，不假设成功一定为 0。打开串口到复位之间可捕获旧固件残留，不纳入本实验结果。

HAL 头文件 stm32c5xx_hal_def.h 定义 HAL_OK=0xEAEAEAEA，即上述十进制值。仓库结构检查通过；自有文件 diff --check 通过。完整导入中的 ST/CMSIS 文件带有既有尾部空格，完整 diff --check 会报告它们；保留供应商原文，未进行格式改写。

## 单组输入限制

最初逐组提交时，只捕获启动行，未完成运算。检查本工程 HAL 源码：`HAL_CORDIC_Calculate` 在写入第一组后设置 index=nb_write-1，随后无条件进入 do/while，再写下一组并 index--。当 nb_write=1 时会多读输入且无符号计数下溢。因此 app 一次提交四组，使用同一官方 HAL API，未修改 vendor 代码。修改后上板得到上述结果。

这是一项源代码定位及应用侧规避，尚未向 ST 提交缺陷，也未证明其他 HAL 版本受影响。
