#include "adc_capture.h"
#include "mx_adc1.h"
#include "stm32c5xx_hal_rcc.h"

static volatile uint32_t complete;
static volatile uint32_t failed;

void HAL_ADC_REG_DataTransferCpltCallback(hal_adc_handle_t *hadc)
{
  if (hadc == mx_adc1_gethandle()) { complete = 1; }
}

void HAL_ADC_ErrorCallback(hal_adc_handle_t *hadc)
{
  if (hadc == mx_adc1_gethandle()) { failed = 1; }
}

int adc_capture_init(void)
{
  if (HAL_ADC_Start(mx_adc1_gethandle()) != HAL_OK) { return -2; }
  if (HAL_ADC_Calibrate(mx_adc1_gethandle()) != HAL_OK) { return -3; }
  return 0;
}

int adc_capture_block(uint32_t *samples, uint32_t count, uint32_t timeout_ms)
{
  if (!samples || count == 0 || count > 4096) { return -1; }
  complete = 0;
  failed = 0;
  if (HAL_ADC_REG_StartConv_DMA(mx_adc1_gethandle(), (uint8_t *)samples, count * sizeof(*samples)) != HAL_OK) {
    return -2;
  }
  uint32_t start = HAL_GetTick();
  while (!complete && !failed && HAL_GetTick() - start < timeout_ms) { __WFI(); }
  int result = failed ? -3 : complete ? 0 : -4;
  if (HAL_ADC_REG_StopConv_DMA(mx_adc1_gethandle()) != HAL_OK) { return -5; }
  __DMB();
  return result;
}
