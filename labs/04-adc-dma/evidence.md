# LAB-004 验证证据

## 已通过

- `cube mx project create-from-board --cpn NUCLEO-C542RC --project-name LAB004_AdcDma --empty-project` 成功创建独立 CubeMX2 源工程。
- `ADC1` 已启用；参数中 `VrefInt/use_channel=true`、规则组软件触发、规则序列包含 VrefInt、`system/dma/dma_enable=true`。
- 通过 `cube mx sw-config` 查询确认上述参数已写入 `.ioc2`。

## 历史阻塞（已解决）

官方 CMake 导出在 `stm32c5xx_hal_mx2codegen` 的 ADC 组件阶段失败，错误为 `TypeError: Cannot convert undefined or null to object`。因此当前没有可信的生成代码、构建、烧录或硬件结果。

上句记录的是初次尝试时状态。2026-11-03 续作启用 HALCore、SystemInit 后，同一 ADC 配置成功导出；此项阻塞已解除。随后通过官方查询将序列通道从错误的 VrefInt 改为 VREFINT，生成了完整的 HAL_ADC_CHANNEL_VREFINT 及采样参数。

## 已验证的实现（2026-11-03）

- CubeMX2 安装包 1.1.2、backend 1.2.0、HAL pack 2.1.0；独立 CMake/Ninja 工程、Arm GNU 14.2.1 构建通过。
- 初次运行 PSIS 启用失败；随后启用 RCC，将 ADC 时钟改为 HCLK 48 MHz / 2，重新导出完整时钟初始化。
- 在已有导出路径重试时发现旧 mx_adc1/mx_system 未更新，尽管导出返回 success。将旧目录可恢复地移至 out/lab004-before-clean 后，在空目标目录重新生成，确认 mx_adc1 使用 HCLK、mx_system 调用 RCC。
- AP1 SWD 1000 kHz、under reset、HWrst，烧录 12.31 KB，Download verified successfully；目标 0x44F、Rev Z、256 KB、3.30 V。
- COM3 115200-8N1，烧录前打开串口，完整启动后捕获连续 7 块：

```text
[LAB-004] block=0 rc=0 valid=64/64 min=1507 max=1509 mean=1508 first=1507 PASS
[LAB-004] block=1 rc=0 valid=64/64 min=1507 max=1509 mean=1508 first=1508 PASS
[LAB-004] block=2 rc=0 valid=64/64 min=1508 max=1509 mean=1508 first=1508 PASS
[LAB-004] block=3 rc=0 valid=64/64 min=1507 max=1509 mean=1508 first=1508 PASS
[LAB-004] block=4 rc=0 valid=64/64 min=1508 max=1509 mean=1508 first=1508 PASS
[LAB-004] block=5 rc=0 valid=64/64 min=1507 max=1509 mean=1508 first=1508 PASS
[LAB-004] block=6 rc=0 valid=64/64 min=1507 max=1509 mean=1508 first=1507 PASS
```

缓冲区每次填充 0xFFFFFFFF；DMA 完成中断后停止 ADC/DMA，再验收全部位置。证明有限长度 DMA 采集功能，不证明循环无缝采样、测量精度或所有故障路径。供应商文件既有尾部空格和 nosys 链接警告保留记录，不改写供应商原文。

随后实际执行 `scripts/Lab004.ps1 -Action Generate -Fresh`：新目录导出、合并和全部 19 个构建步骤成功；再执行脚本 Flash，AP1 校验通过，复位后 block=0..4 均 PASS。仓库结构检查及自有代码/文档空白检查通过。旧导出备份 out/lab004-before-clean 保留，可恢复，未删除源工程。
