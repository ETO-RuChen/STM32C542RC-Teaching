# LAB-013 验证证据（2026-11-03）

- 官方 CubeMX2 新建独立 NUCLEO-C542RC 空工程，HALCore/SystemInit、USART2 已启用；dma list-controllers 返回 LPDMA1/LPDMA2。
- dma bind-resource 创建 LPDMA1_CH0 内存到内存配置，dma_mode=1，启用中断。生成 mx_lpdma1_ch0.c 使用 HAL_DMA_SetConfigLinkedListXfer，事件模式为 Q；HAL 配置开启链表，构建包含 hal_dma.c 与 hal_q.c。
- 初期直接 composer add-component 留下一个未绑定 DMA 实例，使 mx_system.c 出现 `_init()`；使用官方 delete-configuration-instances 删除该空配置，再向全新目的目录生成，确认系统初始化仅包含 USART2 和 LPDMA1_CH0。
- CMake/Ninja、Arm GNU 14.2.1 构建通过；AP1、under reset、HWrst、SWD 1000 kHz，10.80 KB 下载校验通过；0x44F、Rev Z、256 KB、3.30 V。
- COM3 115200-8N1，完整启动后六轮：

```text
[LAB-013] seq=0 nodes=3 words=96/96 guards=6/6 callbacks=1 rc=0 PASS
[LAB-013] seq=1 nodes=3 words=96/96 guards=6/6 callbacks=1 rc=0 PASS
[LAB-013] seq=2 nodes=3 words=96/96 guards=6/6 callbacks=1 rc=0 PASS
[LAB-013] seq=3 nodes=3 words=96/96 guards=6/6 callbacks=1 rc=0 PASS
[LAB-013] seq=4 nodes=3 words=96/96 guards=6/6 callbacks=1 rc=0 PASS
[LAB-013] seq=5 nodes=3 words=96/96 guards=6/6 callbacks=1 rc=0 PASS
```

本证据覆盖静态三个节点线性执行、跨节点数据依赖、每轮数据变化及紧邻边界保护字。未覆盖循环队列、ADC 无缝双缓冲、动态节点、吞吐量或故障注入。供应商原有空白和 nosys 链接警告保持记录，不改写 vendor 源码。

随后加强节点数组完整范围的 64 KiB 窗口检查；实际执行 Lab013.ps1 Generate -Fresh，重新生成并完成 15 个构建步骤。脚本 Flash 再次下载校验 10.79 KB；只截取完整启动行之后的数据，seq=0..4 全部 PASS。仓库结构检查通过（10 个实验契约），自有代码/文档空白检查通过。
