#include "mx_system.h"
#include "mx_usart2.h"
#include "adc_capture.h"
#include <stdio.h>
#include <string.h>

static uint32_t samples[64];

static void log_text(const char *text)
{
  (void)HAL_UART_Transmit(mx_usart2_uart_gethandle(), text, (uint32_t)strlen(text), 1000);
}

int main(void)
{
  if (mx_system_init() != SYSTEM_OK) { for (;;) {} }
  log_text("\r\n[LAB-004] ADC1 VREFINT + LPDMA1_CH0, 64 word block\r\n");
  int init = adc_capture_init();
  if (init != 0) {
    char message[64];
    snprintf(message, sizeof(message), "[LAB-004] init FAIL code=%d\r\n", init);
    log_text(message);
    for (;;) {}
  }
  for (unsigned block = 0;; ++block) {
    for (unsigned i=0; i<64; ++i) { samples[i] = 0xFFFFFFFFu; }
    int result = adc_capture_block(samples, 64, 1000);
    uint32_t min = 0xFFFFFFFFu, max = 0, sum = 0;
    unsigned valid = 0;
    for (unsigned i=0; i<64; ++i) {
      if (samples[i] < min) { min=samples[i]; }
      if (samples[i] > max) { max=samples[i]; }
      if (samples[i] > 0 && samples[i] < 4095) { ++valid; sum+=samples[i]; }
    }
    char line[180];
    snprintf(line, sizeof(line), "[LAB-004] block=%u rc=%d valid=%u/64 min=%lu max=%lu mean=%lu first=%lu %s\r\n",
             block, result, valid, (unsigned long)min, (unsigned long)max,
             (unsigned long)(valid ? sum/valid : 0), (unsigned long)samples[0],
             result == 0 && valid == 64 ? "PASS" : "FAIL");
    log_text(line);
    HAL_Delay(1000);
  }
}
