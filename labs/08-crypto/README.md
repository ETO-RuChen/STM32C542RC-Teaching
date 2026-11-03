# LAB-008：密码外设与隔离（分阶段）

本组实验整体仍为 in-progress。当前已验证 AES-128 ECB 已知答案测试；HASH、RNG、TrustZone 待独立验证。

## AES 第一课：公开标准向量

只需 NUCLEO-C542RC 与 ST-LINK USB，无接线，前置 LAB-002。日志经 USART2/ST-LINK VCP，本机 COM3，115200-8N1。

1. CubeMX2 从 NUCLEO-C542RC 创建 LAB008_Aes 空工程，启用 HALCore、SystemInit。
2. 启用 AES，basic/algorithm=true，aes_standard 保持 AES ECB（值 1）；不在配置中填私有密钥。启用 USART2 Async，PA2/PA3，115200-8N1。
3. 导出 CMake，顶层接入 app、DFP system/startup 和 Flash 链接文件。
4. app/aes_probe.c 设置 NO swapping，通过 HAL_AES_SetNormalKey 加载公开向量密钥，执行加密及独立解密测试。
5. 比较所有四块的参考值、错误密钥对照及六个缓冲区边界保护字，每秒重复。

仓库根目录命令：

```powershell
.\scripts\Lab008-Aes.ps1 -Action Build -Fresh
.\scripts\Lab008-Aes.ps1 -Action Flash
.\scripts\Lab008-Aes.ps1 -Action Generate -Fresh
```

Generate 在全新 out 目录导出后合并，保留 app/顶层 CMake；删除组件时仍检查旧文件和清单。VS Code 打开 LAB008_Aes_cmake，使用 debug_GCC_NUCLEO-C542RC preset，调试选择 AP1。

## 字节序和长度

一个 AES 块为 16 字节。本测试四块共 64 字节，HAL 的 size_byte 参数传 64，不传 word 数 16。

向量以 canonical uint32_t 数值表示：字节序列 6B C1 BE E2 对应数值 0x6BC1BEE2，并设置 NO swapping。小端 MCU 中整数的内存字节序与显示顺序不同，因此 word 数组不能直接作为网络字节串。对接字节流应明确 pack/unpack 或选择一致的 swapping，不能反复试换直到“看起来正确”。每次操作前重新设置 ECB、swapping 和密钥，避免沿用上轮状态。

## 已知答案与错误密钥

参考值来自 [NIST AES 模式示例](https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_ModesA_All.pdf) ECB-AES128 Encryption/Decryption 汇总部分。源码包含公开测试密钥及全部四块数据，不是设备私有秘密。

加密结果必须等于标准密文；解密输入使用独立参考密文，而非刚刚计算的结果。仅“加密再解密能回来”可能漏掉两端相互抵消的表示错误。

```text
[LAB-008/AES] seq=0 enc=4/4 dec=4/4 wrong_key_diff=1 guards=6/6 rc=0 PASS
[LAB-008/AES] cipher0=3ad77bb40d7a3660a89ecaf32466ef97
```

换错测试密钥后 HAL 解密依旧成功，但结果不等于参考明文。这说明 ECB 不提供真实性认证；真实消息通常没有已知明文可供 memcmp，不能照搬此检查作为完整性验证。ECB 在这里用于块算法硬件验证；OTA/业务通信后续再设计认证、密钥管理和镜像验证。

## 调试、恢复与练习

rc=-1/-3/-5：三个阶段配置失败；-2：加密失败；-4：正确密钥解密失败；-6：错误密钥对照的 HAL 调用失败；-7：向量/对照/保护字不符。每次运算超时 100 ms，失败后停止保留现场。六个保护字只检查紧邻边界，不证明所有内存行为安全。

观察 AES 控制/状态寄存器、输出 word、swapping 和 HAL 状态。串口无输出先查 COM 占用、波特率和 AP1 烧录；结果不匹配先核对长度/字节序/模式/向量，不能用实测输出反改预期值。snprintf 引入 nosys 文件 I/O 链接警告，日志直接用 HAL_UART_Transmit。

练习改为重复块观察 ECB 图样；改变输入位后重新选择对应标准向量；添加 AES-256 或其他模式的官方向量。GCM/CCM、DMA、性能、密钥保护和侧信道尚未验证。此接口仅支持单调用者公开测试，不是生产密码服务。

参考：[ST HAL AES 2.1.0 概览](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/2.1.0/en/docs/drivers/hal_drivers/aes/hal_aes_overview.html)。具体 API 以签入的 HAL 2.1.0 为准；提交历史见 `git log -- labs/08-crypto`。下一阶段为 HASH 已知摘要测试。
