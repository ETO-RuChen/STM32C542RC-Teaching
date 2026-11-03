# LAB-006：CORDIC 定点三角函数

目标：用芯片内部 CORDIC 同时计算 cos 和 sin，理解角度归一化、Q1.31、精度和批量处理。前置实验是 LAB-002；只需开发板及 ST-LINK USB，不需要接线。串口使用 ST-LINK VCP（本机 COM3），115200、8N1。

## 人工开发步骤

1. CubeMX2 选择 NUCLEO-C542RC，创建独立空工程 `LAB006_Cordic`，保存 `.ioc2`。
2. 启用全局服务 HALCore、SystemInit。空工程不会自动包含这两项；缺少它们时，当前生成器在 CORDIC/UART 阶段抛出空对象异常，详见 evidence.md。
3. 启用 CORDIC：COSINE、32 位输入输出、两个参数、两个结果、scale=0、precision=6。
4. 启用 USART2 Async，PA2/PA3，115200、8N1、无流控。
5. 导出 CMake，保留本地软件包。顶层 CMake 添加项目自有 `app/main.c`，添加 DFP 自带启动文件、system 文件及 Flash 链接脚本。
6. 编译、烧录、打开串口，检查四组运算及汇总。

在仓库根目录运行：

```powershell
.\scripts\Lab006.ps1 -Action Generate -Fresh
.\scripts\Lab006.ps1 -Action Build -Fresh
.\scripts\Lab006.ps1 -Action Flash
```

Generate 用于已签入工程的重新生成，采用 keep_user 保留顶层 CMake 的自有源文件清单。若从全新目的目录生成，需要完成第 5 步。构建产物在 `LAB006_Cordic_cmake/build/debug_GCC_NUCLEO-C542RC/`。可在 VS Code 打开 CMake 目录；使用相同 preset 构建，调试连接选 AP1。

## 运算规则

Q1.31 的真实数值 = 有符号整数 / 2^31，可表示 -1 到接近 1。角度输入是 angle/π；π/2 对应 0x40000000，π/4 对应 0x20000000。第二个参数为模长 0.5，即 0x40000000，输出因此是 0.5*cos(angle)、0.5*sin(angle)。采用半幅值可以避免 +1 端点饱和。

| 角度 | 期望 cos Q1.31 | 期望 sin Q1.31 |
|---|---:|---:|
| 0 | 1073741824 | 0 |
| π/4 | 759250125 | 759250125 |
| π/2 | 0 | 1073741824 |
| -π/2 | 0 | -1073741824 |

每个分量容差为 4096 个整数单位，折算约 1.91e-6；这只验证这四个向量，不代表整个输入域误差上限。6 cycles 是 CORDIC 配置值，不是整个 HAL 调用或串口输出耗时。

## 代码与练习

配置和 HAL 驱动分别在 generated/hal 和 stm32c5xx_drivers；app/main.c 持有测试向量、HAL 缓冲区描述及验收逻辑。当前为一个独立入门实验，尚未提取跨项目公共数学服务。

练习：增加 -π/4 向量并同步扩大缓冲区；降低 precision 观察误差；将模长改为 0.25 并推导新的期望值。每次修改都重新运行串口自检。

## 限制与恢复

- HAL 2.1.0 `HAL_CORDIC_Calculate` 的单组输入路径存在循环边界问题；本实验一次提交四组输入（8 words），输出 8 words。不要改为逐组调用；源代码依据见 evidence.md。
- 使用 snprintf 会引入 nosys 的 _read/_write/_close/_lseek 链接警告。日志实际使用 HAL_UART_Transmit；未配置文件 I/O。
- 若无输出，检查串口占用、波特率及 AP1 烧录结果；在复位前打开串口，忽略旧固件的复位前残留字节。
- 这是数值功能实验，尚未进行性能基准、DMA 模式或全输入域误差扫描。后续再扩展数学服务和性能测量。

提交记录通过 `git log -- labs/06-cordic` 查阅。
