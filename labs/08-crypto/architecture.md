# AES 阶段边界

main.c 管理周期/日志/失败停机；aes_probe.c 持有公开向量、缓冲区、配置和完整比较；generated/hal 管理初始化，vendor HAL/LL 操作外设。

探针独占 AES，轮询运算每次最多等待 100 ms，返回匹配块数、错误密钥对照和保护字结果。测试 key 及缓冲区有静态生命周期。当前不提供认证解密或秘密存储。HASH/RNG/TrustZone 将分别验证，再确定公共服务及隔离边界。
