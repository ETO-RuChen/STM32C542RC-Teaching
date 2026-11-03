# LAB-007 证据（2026-11-03）

- 官方 CubeMX2 创建独立 NUCLEO-C542RC 空工程，HALCore/SystemInit、FDCAN1、USART2 均通过 CLI 配置。
- 查询 mode 的合法值后选择 INTERNAL_LOOPBACK，经典 CAN，48 MHz PCLK1，500 kbps；标准过滤器分配 1 个，非匹配标准/扩展帧及远程帧拒绝。
- 生成 mx_fdcan1.c：prescaler=1、seg1=83、seg2=12、SJW=1，PA11/PA12 AF9。过滤器内容由自有适配层填写。
- CMake/Ninja、Arm GNU 14.2.1 构建通过。AP1、under reset、HWrst、1000 kHz，10.87 KB 下载并校验成功；0x44F、Rev Z、256 KB、3.30 V。
- COM3 115200-8N1，在完整 LAB-007 启动行之后采集五轮：

```text
[LAB-007] seq=0 accept_0x123+payload reject_0x321 rc=0 PASS
[LAB-007] seq=1 accept_0x123+payload reject_0x321 rc=0 PASS
[LAB-007] seq=2 accept_0x123+payload reject_0x321 rc=0 PASS
[LAB-007] seq=3 accept_0x123+payload reject_0x321 rc=0 PASS
[LAB-007] seq=4 accept_0x123+payload reject_0x321 rc=0 PASS
```

每轮均确认发送完成、匹配帧头和载荷，以及不匹配 ID 在发送成功后未进入 FIFO0。它证明本配置的内部回环与过滤功能，不证明外部 CAN 总线或 FD/BRS 功能。复位前串口旧固件残留不纳入验收。

再执行 Lab007.ps1 Generate -Fresh，新目录导出与全部 16 个构建步骤通过；脚本 Flash 再次校验成功，完整复位启动行后 seq=0..3 全部 PASS。仓库结构检查通过（9 个实验契约），自有代码/文档空白检查通过。供应商原有尾部空格及 nosys 文件 I/O 链接警告保留，未改写 vendor 文件。
