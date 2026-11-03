#include "can_loopback.h"
#include "mx_fdcan1.h"
#include <string.h>

static int send_frame(uint32_t id, const uint8_t *data)
{
  hal_fdcan_handle_t *h = mx_fdcan1_gethandle();
  hal_fdcan_tx_header_t header = {.d64 = 0};
  /* HAL 2.1.0 API shifts standard IDs internally; pass the unshifted 11-bit ID. */
  header.b.identifier = id;
  header.b.identifier_type = HAL_FDCAN_ID_STANDARD;
  header.b.frame_type = HAL_FDCAN_FRAME_DATA;
  header.b.frame_format = HAL_FDCAN_HEADER_FRAME_FORMAT_CAN;
  header.b.data_length = HAL_FDCAN_DATA_LEN_CAN_FDCAN_8_BYTE;
  if (HAL_FDCAN_ReqTransmitMsgFromFIFOQ(h, &header, data) != HAL_OK) { return -1; }
  uint32_t buffer = HAL_FDCAN_GetLatestTxFifoQRequestBuffer(h);
  uint32_t start = HAL_GetTick();
  while (HAL_FDCAN_GetTxBufferMessageStatus(h, buffer) != HAL_FDCAN_TX_BUFFER_NOT_PENDING) {
    if (HAL_GetTick() - start >= 100) { return -2; }
  }
  return (FDCAN1->TXBTO & buffer) ? 0 : -3;
}

int can_loopback_init(void)
{
  hal_fdcan_filter_t filter = {0};
  filter.id_type = HAL_FDCAN_ID_STANDARD;
  filter.filter_index = 0;
  filter.filter_type = HAL_FDCAN_FILTER_TYPE_CLASSIC;
  filter.filter_config = HAL_FDCAN_FILTER_TO_RX_FIFO_0;
  filter.filter_id1 = 0x123;
  filter.filter_id2 = 0x7FF;
  if (HAL_FDCAN_SetFilter(mx_fdcan1_gethandle(), &filter) != HAL_OK) { return -1; }
  return HAL_FDCAN_Start(mx_fdcan1_gethandle()) == HAL_OK ? 0 : -2;
}

int can_loopback_round(uint32_t sequence)
{
  uint8_t tx[8] = {0xCA,0x54,(uint8_t)sequence,(uint8_t)(sequence>>8),0x11,0x22,0x33,0x44};
  uint8_t rx[64] = {0};
  hal_fdcan_rx_header_t header = {.d64 = 0};
  hal_fdcan_handle_t *h = mx_fdcan1_gethandle();
  if (send_frame(0x123, tx) != 0) { return -1; }
  uint32_t fill = 0;
  uint32_t start = HAL_GetTick();
  do {
    HAL_FDCAN_GetRxFifoFillLevel(h, HAL_FDCAN_RX_FIFO_0, &fill);
    if (HAL_GetTick()-start >= 100) { return -2; }
  } while (!fill);
  if (HAL_FDCAN_GetReceivedMessage(h, HAL_FDCAN_RX_FIFO_0, &header, rx) != HAL_OK) { return -3; }
  if (header.b.identifier != 0x123 || header.b.identifier_type != HAL_FDCAN_ID_STANDARD ||
      header.b.data_length != HAL_FDCAN_DATA_LEN_CAN_FDCAN_8_BYTE || header.b.filter_index != 0 ||
      header.b.frame_type != HAL_FDCAN_FRAME_DATA || header.b.frame_format != HAL_FDCAN_HEADER_FRAME_FORMAT_CAN ||
      memcmp(tx, rx, sizeof(tx)) != 0) { return -4; }
  if (send_frame(0x321, tx) != 0) { return -5; }
  start = HAL_GetTick();
  do {
    HAL_FDCAN_GetRxFifoFillLevel(h, HAL_FDCAN_RX_FIFO_0, &fill);
    if (fill) { return -6; }
  } while (HAL_GetTick()-start < 20);
  return 0;
}
