# LAB-001：板载 LED 呼吸/闪烁与按键切换

## 目标

不用外接器件，仅使用 NUCLEO-C542RC 板载资源完成第一个正式工程：

- LD1（PA5）默认以亮 1 秒、灭 1 秒的节奏闪烁；
- 每次稳定按下 B1（PC13）切换到另一模式；
- 呼吸模式约 2 秒渐亮、2 秒渐暗，使用 200 Hz 软件 PWM；
- 闪烁和呼吸是两个互斥状态，不叠加。

## 工程组成

`LAB001_LedButton.ioc2` 是 CubeMX2 配置源；`LAB001_LedButton_cmake/` 是完整 CMake/HAL 工程。生成代码和项目自有代码的边界如下：

```text
CubeMX2 / vendor
  generated/ + stm32c5xx_* + part_drivers/
                |
platform/cubemx2_compat
                |
bsp/bsp_led + bsp/bsp_button
                |
app/app_led_modes
```

应用通过 CubeMX2 的 `post_system_init_hook()` 启动。时钟、HAL、ICACHE、GPIO 先由生成代码初始化，之后才进入应用的非阻塞前台循环。

## GPIO 配置

| 资源 | 引脚 | CubeMX2 模式 | 电气配置 |
|---|---|---|---|
| LD1 | PA5 | Output | Push-pull、Low speed、No pull、初始低 |
| B1 | PC13 | Input | No pull、轮询、软件消抖 20 ms |

首版故意不用 EXTI 或硬件定时器：先把状态机、时间差写法、消抖和分层边界讲清楚。定时器 PWM 会在 LAB-003 单独学习。

## 生成、构建与烧录

在仓库根目录执行：

```powershell
# 从 ioc2 重新生成，并立即构建
.\scripts\Lab001.ps1 -Action Generate

# 只构建
.\scripts\Lab001.ps1 -Action Build

# 构建、使用唯一的 ST-LINK，经 AP1 烧录、校验并复位
.\scripts\Lab001.ps1 -Action Flash
```

需要清理 CMake 缓存时加 `-Fresh`。不要把 AP 改成 AP0；本板已经实测 AP1 可用。

## 核心知识

### 1. 用无符号减法处理 tick 回绕

所有周期判断均写成 `now - last >= interval`。在 `uint32_t` 上，这种写法能自然跨过 tick 溢出。

### 2. 按键消抖只产生边沿事件

`bsp_button` 每 5 ms 采样。电平连续稳定 20 ms 后才接受新状态，并且只在稳定的“按下”边沿返回一次事件；一直按住不会反复切换。

### 3. 两个模式互斥

应用只有一个 `mode` 枚举。模式切换时统一重置闪烁计时、呼吸斜坡和 PWM 相位，因此不会残留上一模式的闪烁节奏。

### 4. 软件 PWM 只是教学过渡

一个 PWM 帧有 100 个 50 us 时隙，总周期 5 ms，即 200 Hz。亮度每 20 ms 改变一级，共 100 级。该实现适合单个 LED 的入门演示，不用于多通道或低功耗产品；后续用定时器 PWM 和 DMA 替换。

## 验收步骤

1. 上电或复位后，观察 LD1 亮约 1 秒、灭约 1 秒。
2. 短按 B1 一次，LD1 应进入连续、无额外闪断的呼吸效果。
3. 再短按一次，回到 1 秒亮/1 秒灭。
4. 长按 B1，模式只应切换一次；松开不应再次切换。
5. 快速连续按键时，每个经过 20 ms 消抖的独立按下动作应只产生一次切换。

## 已记录验证

- 完整 CMake 构建通过，ELF 的 text/data/bss 为 7760/96/1928 字节（200 Hz 调整前后代码规模同量级）。
- CubeProgrammer 2.23.0 通过 ST-LINK AP1 识别 Device ID `0x44F`。
- 固件下载 7.67 KiB，校验成功，复位成功。
- 调试采样得到 PA5 输出 `1 → 0 → 1`，间隔约 1.1 秒，系统 tick 同步增长。
- 调试注入呼吸模式后，亮度级别连续变化且 PWM 相位运行；随后已复位恢复默认模式。
- 因操作时无人按板载 B1，物理按键手感与一次一切换仍保留为上面的人工复核项，不能用构建结果替代。

## 故障排查

- `No STM32 target found`：确认命令含 `ap=1`，并检查板上 ST-LINK USB 连接。
- `mx_button.h: No such file`：这是当前 CubeMX2 1.1.2 的 Board Part 导出缺陷；项目通过 `platform/cubemx2_compat/mx_button.h` 补齐官方生成接口。
- LED 完全不亮：先确认生成的 `mx_gpio_default.c` 把 PA5 配成输出，再检查 LD1 是否被跳线或外部硬件占用。
- 呼吸看起来仍闪：确认正在运行本实验最新固件；PWM 应为 200 Hz，不是 1 秒闪烁状态机叠加。
- 按键方向相反：不要直接改应用策略，先核对板卡版本与 CubeMX2 Board Part 的 PC13 active level。

## 下一实验

LAB-002 使用 ST-LINK VCP / USART2 建立结构化日志和可观察性基础，为后续 DMA、FreeRTOS、IAP/OTA 和企业级诊断打底。
