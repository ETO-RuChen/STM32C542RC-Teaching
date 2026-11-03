#ifndef ADC_CAPTURE_H
#define ADC_CAPTURE_H
#include <stdint.h>
int adc_capture_init(void);
int adc_capture_block(uint32_t *samples, uint32_t count, uint32_t timeout_ms);
#endif
