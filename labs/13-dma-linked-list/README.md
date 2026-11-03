# LAB-013：LPDMA 三节点线性链表

本课已通过板上自检。只需 NUCLEO-C542RC 与 ST-LINK USB，不需要接线。前置 LAB-002 和 LAB-004。当前芯片的控制器名称是 LPDMA1/LPDMA2，本课使用 HAL 2.1.0 的 LPDMA 链表 API，不套用其他系列的 GPDMA/DMAEx API。

## 从 CubeMX2 到可开发工程

1. 从 NUCLEO-C542RC 创建独立 LAB013_DmaLinkedList 空工程，启用 HALCore、SystemInit。
2. 启用 USART2 Async，PA2/PA3、115200-8N1。
3. 在 DMA 页面绑定 LPDMA1_CH0 为内存到内存通道。CLI 对应 `cube mx dma bind-resource -p labs/13-dma-linked-list/LAB013_DmaLinkedList.ioc2 --channel LPDMA1_CH0`。这会创建绑定的配置，不能直接通过 peripherals enable LPDMA1 启用。
4. 将通道 dma_mode 设置为 Linked list transfer（CLI 值 1），32 位宽度，并启用中断。生成配置开启 USE_HAL_DMA_LINKEDLIST、HAL_Q 支持和队列完成事件。
5. 导出 CMake，顶层接入 app、DFP system/startup 和 Flash 链接脚本；项目自有代码构建队列并填写每个节点的传输配置。
6. 编译、烧录，ST-LINK VCP 查看结果。本机 COM3，115200-8N1。

仓库根目录命令：

```powershell
.\scripts\Lab013.ps1 -Action Build -Fresh
.\scripts\Lab013.ps1 -Action Flash
.\scripts\Lab013.ps1 -Action Generate -Fresh
```

Generate 使用新的 out 导出目录，合并生成内容，保留自有 app 和顶层 CMake。删除组件时仍需检查旧组件文件。VS Code 打开 LAB013_DmaLinkedList_cmake，使用 debug_GCC_NUCLEO-C542RC preset；调试使用 AP1。

## 真正由硬件串起来的三个节点

| 节点 | 源 | 目的 | 大小 |
|---|---|---|---|
| 0 | source_a | temporary.data | 32 words / 128 bytes |
| 1 | source_b | destination_b.data | 32 words / 128 bytes |
| 2 | temporary.data | destination_c.data | 32 words / 128 bytes |

CPU 用 HAL_DMA_FillNodeDirectXfer 和 HAL_DMA_FillNodeData 描述每次搬运，再用 HAL_Q_InsertNode_Tail 依次连接。每轮只调用一次 HAL_DMA_StartLinkedListXfer_IT，硬件读取后续节点并执行，CPU 不逐节点重启 DMA，也不代做 memcpy。

第三个节点的源来自第一个节点的输出。每轮先把目的区全部填为 0xFFFFFFFF，再变化两个源数组的模式；最后逐字验证 temporary=source_a、destination_b=source_b、destination_c=source_a。这样既检查三个目的区，也检查先生产临时数据、后消费的顺序。

## 内存和完成时机

节点和缓冲区均为静态对象，DMA 完成之前不会离开作用域。节点数组首地址按 32 字节对齐，每个描述符满足字对齐；初始化还检查整个数组位于同一 64 KiB 地址窗口。传输长度是 **字节**，32 个 uint32_t 应传 128，不能传 32。

每个目的区前后各放一个保护字，共六个。它们只能发现紧邻缓冲区的越界，不是完整的内存安全证明。主循环等到 DMA 队列完成回调后再读数据，中断只计数完成或记录错误。启动前及完成后使用 DMB；本项目没有启用数据缓存，迁移到其他缓存配置时必须重新处理 DMA 可见性，DMB 本身不等于清理缓存。

回调事件模式为 Q，因此三个节点完成后只应回调一次。节点/队列在运行期间保持不变；当前是单通道、单调用者、线性有限队列，每秒重复启动，不是循环双缓冲。

## 验收和调试

```text
[LAB-013] seq=0 nodes=3 words=96/96 guards=6/6 callbacks=1 rc=0 PASS
```

必须同时满足 96 个数据字正确、6 个保护字不变、恰好 1 次完成回调。运行等待最长 100 ms，错误或超时后尝试 Abort，随后应用停在现场供调试。rc=-1 表示启动失败，-2 表示错误回调，-3 表示超时，-4 表示数据/边界/完成次数不符。

可在调试器检查 queue、nodes[].regs、LPDMA1_CH0 的 CLLR/CLBAR/CSAR/CDAR/CBR1 与目的缓冲区。暂停可能扰动超时，不把单步状态当作正常性能结果。串口无输出先检查 COM 占用与 AP1 烧录。

配置过程中如果直接添加一个未绑定的 DMA 组件，可能产生空的 `_init()` 调用。应使用资源绑定创建通道，并在导出前删除多余未绑定配置；本课曾复现并清除了该空配置，生成文件保持原文。snprintf 引入 nosys 文件 I/O 链接警告；日志实际使用 HAL_UART_Transmit。

练习增加第四个节点、改变各块长度并相应扩大验收；加入可控数据破坏，确认自检报告 FAIL；随后再探索 circular queue、静态/动态节点、外设请求和 ADC 双缓冲。当前未验证这些扩展，也未测量吞吐量或全部错误恢复路径。

参考：[ST HAL DMA 2.1.0 使用说明](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.1.0/en/docs/drivers/hal_drivers/dma/hal_dma_how_to_use.html)。具体 API 以工程签入的 HAL 2.1.0 为准。提交历史见 `git log -- labs/13-dma-linked-list`。
