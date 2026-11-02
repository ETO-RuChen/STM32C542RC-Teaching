# Architecture Roadmap

## Teaching baseline

core -> hal -> driver -> bsp -> middleware -> rtos -> app

CubeMX-generated and vendor code stays at the HAL/vendor boundary; board pin mapping stays in BSP; application behavior stays in labs/apps.

## Mature project shape

vendor -> platform -> drivers -> services -> osal/rtos -> app

This transition happens only after at least two labs reuse a module and its interface has a test or board verification record.

## Upgrade and security boundary

bootloader -> image validation -> update service -> transport adapter

UART is the first transport for IAP/OTA demonstrations. USB or a future network adapter may be added without changing image validation or rollback policy. TrustZone crypto services must not expose key material to the non-secure application.
