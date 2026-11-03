# LAB-004：ADC 内部 VrefInt + DMA

已通过编译、AP1 烧录校验及 COM3 运行验证。前置实验为 LAB-002。只需 NUCLEO-C542RC 及 ST-LINK USB，无外部接线。

## 人工开发流程

1. CubeMX2 从 NUCLEO-C542RC 创建独立空工程 LAB004_AdcDma，启用 HALCore、SystemInit 和 RCC。
2. 保持 SYSCLK/HCLK 48 MHz；ADC_DAC_Clock_Source 选 HCLK1，ADCDACPRE_DIV=2，ADC 时钟 24 MHz。
3. 启用 ADC1 内部 VrefInt，12 位、289 周期采样；规则序列长度 1，通道从查询选项选 `VREFINT`，软件触发、连续转换。
4. 启用 ADC DMA，分配 LPDMA1_CH0：外设到内存、源固定、目的递增、32 位数据宽度，启用 DMA 中断。
5. 启用 USART2 Async，PA2/PA3，115200-8N1；导出 CMake。顶层 CMake 接入自有 app、DFP system/startup 文件及 Flash 链接脚本。
6. 编译、烧录，串口观察每秒一个 64 样本块的统计结果。

仓库根目录命令：

```powershell
.\scripts\Lab004.ps1 -Action Build -Fresh
.\scripts\Lab004.ps1 -Action Flash
.\scripts\Lab004.ps1 -Action Generate -Fresh
```

Generate 用于已签入的完整工程：先在 out 下全新导出，再更新内容，保留自有顶层 CMake 和 app。若删除组件，应检查旧组件文件与构建清单；合并导出不会自动清除旧文件。out 中导出副本不提交 Git，可用于恢复或比较。

在 VS Code 打开 LAB004_AdcDma_cmake，使用同名 preset；构建产物在 build/debug_GCC_NUCLEO-C542RC/。ST-LINK 调试连接选择 AP1。

## DMA 和缓冲区

ADC 完成转换后产生 DMA 请求，LPDMA 将 ADC 数据寄存器搬入内存；CPU 不逐个读取 ADC 数据。main.c 拥有 64 个 uint32_t 样本，共 256 字节，传给 HAL_ADC_REG_StartConv_DMA 的长度单位是字节。

应用先用 0xFFFFFFFF 填充缓冲区，adc_capture_block 阻塞等待 DMA 完成回调，最长 1000 ms，随后停止转换/DMA，再将缓冲区交回统计逻辑。中断只设置标志；不发送串口、不统计。等待时 WFI，SysTick 继续提供超时检查。

这是周期性有限长度采集：块内连续采样，块间等待约 1 秒，不提供无间隙循环采集。后续通过链表/双缓冲和队列扩展。

## 验收与练习

ST-LINK VCP 本机为 COM3，115200-8N1。典型输出：

```text
[LAB-004] block=0 rc=0 valid=64/64 min=1507 max=1509 mean=1508 first=1507 PASS
```

PASS 要求采集成功，且所有位置成为非零、非饱和的 12 位数值。连续 7 块实测为 1507～1509。min/max/mean 用于观察稳定性，不代表校准后的电压精度；不同板卡、温度、供电不应要求完全相同原始值。

可在调试器观察 samples、完成标志、ADC1 和 LPDMA1_CH0；暂停会影响正常采样与超时。练习改变块长度，保持字节长度正确；增加峰峰值或均值统计；降低采样时间观察变化。

## 故障与恢复

- 空对象生成错误：先检查 HALCore/SystemInit，本工程补齐后生成成功。
- 生成无效 `HAL_` 常量：规则序列的有效选项为 `VREFINT`，不同于通道配置列表里的 `VrefInt`。通过 get-parameter-possible-values 查询实际选项。
- 旧导出保留 PSIS：使用新目录导出。检查 mx_adc1.c 选择 HCLK，mx_rcc.c 设置 DIV2，mx_system.c 调用 RCC。
- rc=-2：DMA 启动失败；-3：错误回调；-4：超时；-5：停止失败。检查时钟、DMA 分配、中断、缓冲区与长度。
- 初版 PSIS 启用失败，现改为 RCC 初始化 HCLK/2；不依赖该振荡器配置。
- snprintf 引入 nosys 文件 I/O 链接警告；日志直接使用 HAL_UART_Transmit。
- 复位前打开串口可能收到旧固件残留，验收从完整 LAB-004 启动行开始。

该采集适配 API 仅支持单调用者，跨实验复用前还需并发、取消与恢复契约。提交历史见 `git log -- labs/04-adc-dma`；下一实验为 DMA 链表或高级定时器。
