# LAB-007：FDCAN 内部回环与接收过滤

本课通过 FDCAN1 的内部回环验证经典 CAN 发送、接收、FIFO 和过滤，不需要额外接线。前置 LAB-002；硬件只需 NUCLEO-C542RC 和 ST-LINK USB，日志走 ST-LINK VCP（本机 COM3、115200-8N1）。

## 手工开发步骤

1. CubeMX2 从 NUCLEO-C542RC 创建 LAB007_FdcanLoopback 空工程；启用 HALCore、SystemInit。
2. 启用 FDCAN1，mode=INTERNAL_LOOPBACK，frame_format=CLASSIC_CAN。内核时钟选 PCLK1=48 MHz，clock divider=1，目标 nominal bitrate=500 kbps。
3. 分配 1 个标准过滤器、0 个扩展过滤器；不匹配标准帧和扩展帧全部拒绝，远程帧拒绝。应用启动前配置 filter 0 为 0x123/0x7FF，接收位置 FIFO0。
4. 启用 USART2 Async，PA2/PA3、115200-8N1；导出 CMake，接入 app 和 DFP 的 system/startup/Flash 链接文件。
5. HAL 系统初始化后设置过滤器，再 HAL_FDCAN_Start；逐条发送并确认 TX 完成，接收并检查报文，再做不匹配 ID 拒收检查。

仓库根目录运行：

```powershell
.\scripts\Lab007.ps1 -Action Build -Fresh
.\scripts\Lab007.ps1 -Action Flash
.\scripts\Lab007.ps1 -Action Generate -Fresh
```

Generate 用全新 out 目录导出后合并，保留自有 app 和顶层 CMake；删除组件时检查旧文件和清单。VS Code 打开 LAB007_FdcanLoopback_cmake，选择 debug_GCC_NUCLEO-C542RC preset；调试使用 AP1。

## 位时序和报文

导出配置为 prescaler=1、seg1=83、seg2=12、SJW=1。每 bit 为 1+83+12=96 个 time quanta，48 MHz/96=500 kbit/s；采样点为 (1+83)/96=87.5%。这里验证内部控制器功能，不把配置数值当成外部物理层测量。

标准 CAN ID 为 11 位。经典 CAN 的本实验数据长度为 8 字节，数据中包含不断变化的序号，防止旧报文被误认成当前响应。HAL 2.1.0 的 tx/rx header 是 union/位域；本版本 API 内部处理标准 ID 的移位，因此调用者传入/读取 0x123，不自行左移 18 位。迁移其他 HAL 版本时必须重新检查 API。

接收规则：`(received_id & 0x7FF) == (0x123 & 0x7FF)`。匹配进入 FIFO0；不匹配由全局规则拒绝。分配过滤器数量与填写过滤器内容是两步，生成配置只分配 1 个槽，实际内容由 can_loopback_init 写入。

## 自检过程

每轮发送 0x123，等待最多 100 ms，确认 TX 请求不再 pending 且 TXBTO 对应位置位；再等 FIFO0 接收，验证 ID、类型、DLC、filter index 和全部载荷。之后发送 0x321，同样确认发送完成，再连续观察 20 ms FIFO0 为空。接收前使用 64-byte 缓冲区避免后续扩展 DLC 时缓冲区过小。

```text
[LAB-007] seq=0 accept_0x123+payload reject_0x321 rc=0 PASS
```

rc=0 才通过。-1：接受帧发送失败；-2：接收超时；-3：读取失败；-4：头部或载荷不符；-5：拒收帧发送失败；-6：不匹配帧意外进入 FIFO。失败后保留现场供调试，不反复发送掩盖错误。底层发送也有超时，内部各错误映射到上述阶段码。

## 引脚与范围

CubeMX2 自动分配 PA11/PA12 AF9 为 FDCAN RX/TX，这对引脚也带有板级 USB_FS_N/P 标签。本课使用内部回环，不接 CAN 总线，也不同时运行目标 USB 功能。ST-LINK USB/VCP 是独立调试通路。

内部回环通过不能证明外部收发器、双绞线、终端电阻、节点仲裁、总线故障恢复或实际比特率通过。经典 CAN 已验证；FD 长帧、BRS、中断接收、CANopen/其他上层协议属于后续内容。实际总线通信需要适配的收发器和布线，本课不要求这些设备。

## 调试与练习

观察 FDCAN1 CCCR/TEST（内部回环）、NBTP、SIDFC、GFC、TXBTO、RXF0S 和 app 序号。串口无输出先检查端口占用和 AP1 烧录。只分配过滤器却未填写时，合法帧也可能收不到；设置全局拒绝后尤其要检查 filter 0。

练习将允许 ID 改为 0x456，保持正反测试各自期望；将 mask 改为 0x7F0 并设计允许一组 ID 的案例；改变载荷并确认 memcmp 实际检查全部 8 字节。使用 snprintf 会引入 nosys 文件 I/O 链接警告，日志实际通过 HAL_UART_Transmit。

自有 can_loopback.c 是实验内单调用者适配，main.c 承担日志与验收；尚未抽取通用 CAN 服务。下一步再加入接收中断、队列及 OSAL。Git 历史：`git log -- labs/07-fdcan-loopback`。

参考：[ST HAL FDCAN 2.1.0 使用说明](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.1.0/en/docs/drivers/hal_drivers/fdcan/hal_fdcan_how_to_use.html)、[RM0522 STM32C5 参考手册](https://www.st.com/resource/en/reference_manual/rm0522-stm32c5-series-armbased-32bit-mcus-stmicroelectronics.pdf)。具体 API 以本工程签入的 HAL 2.1.0 为准。
