# AES 阶段证据（2026-11-03）

- CubeMX2 独立 NUCLEO-C542RC 空工程，HALCore/SystemInit、AES ECB、USART2；完整 CMake 导出成功。
- HAL 2.1.0、Arm GNU 14.2.1、CMake/Ninja 构建通过。驱动源码按 key word 顺序写 KEYR3..KEYR0；使用 canonical uint32_t 数值与 NO swapping。
- AP1、under reset、HWrst、1000 kHz，10.50 KB 下载校验成功；0x44F、Rev Z、256 KB、3.30 V。
- [NIST 原始向量](https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/AES_ModesA_All.pdf) ECB-AES128 汇总部分作为独立预期值。
- COM3 115200-8N1，完整启动后 seq=0..5 六轮均为：

```text
[LAB-008/AES] seq=0 enc=4/4 dec=4/4 wrong_key_diff=1 guards=6/6 rc=0 PASS
[LAB-008/AES] cipher0=3ad77bb40d7a3660a89ecaf32466ef97
```

逐块比较全部 64 字节加密结果；解密输入为独立参考密文。错误公开测试密钥的解密调用成功，但结果不同；六个保护字不变。

证据覆盖本组 AES-128 ECB 已知答案，不证明其他模式、密钥保护、认证安全或侧信道属性。HASH/RNG/TrustZone 尚未测试。

随后将错误公开测试 key 缓冲区也改为静态生命周期；实际执行 Lab008-Aes.ps1 Generate -Fresh，重新导出并完成全部 16 个构建步骤。脚本 Flash 再次校验 10.51 KB；仅统计完整启动行之后的数据，seq=0..4 全部 PASS。仓库结构检查通过（11 个实验契约），自有文件空白检查通过。供应商既有空白与 nosys 链接警告按教程保留。
