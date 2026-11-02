# LAB-004 验证证据

## 已通过

- `cube mx project create-from-board --cpn NUCLEO-C542RC --project-name LAB004_AdcDma --empty-project` 成功创建独立 CubeMX2 源工程。
- `ADC1` 已启用；参数中 `VrefInt/use_channel=true`、规则组软件触发、规则序列包含 VrefInt、`system/dma/dma_enable=true`。
- 通过 `cube mx sw-config` 查询确认上述参数已写入 `.ioc2`。

## 当前阻塞

官方 CMake 导出在 `stm32c5xx_hal_mx2codegen` 的 ADC 组件阶段失败，错误为 `TypeError: Cannot convert undefined or null to object`。因此当前没有可信的生成代码、构建、烧录或硬件结果。
