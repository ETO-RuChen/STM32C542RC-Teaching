#include "mx_system.h"
#include "mx_cordic.h"
#include "mx_usart2.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* Q1.31 angle encodes angle/pi. Modulus 0.5 keeps outputs away from saturation. */
static const struct {
  int32_t angle;
  int32_t cosine;
  int32_t sine;
  const char *label;
} vectors[] = {
  {0, 1073741824, 0, "0"},
  {536870912, 759250125, 759250125, "pi/4"},
  {1073741824, 0, 1073741824, "pi/2"},
  {-1073741824, 0, -1073741824, "-pi/2"},
};

static void log_line(const char *s)
{
  (void)HAL_UART_Transmit(mx_usart2_uart_gethandle(), s, (uint32_t)strlen(s), 1000);
}

static int near(int32_t actual, int32_t expected)
{
  int64_t delta = (int64_t)actual - expected;
  return delta >= -4096 && delta <= 4096;
}

int main(void)
{
  if (mx_system_init() != SYSTEM_OK) { for (;;) {} }
  log_line("\r\n[LAB-006] CORDIC Q1.31 cosine+sine, modulus=0.5\r\n");
  for (;;) {
    unsigned passed = 0;
    /* HAL 2.1.0 polling pipeline requires at least two calculations. */
    int32_t input[8];
    int32_t output[8] = {0};
    for (unsigned i = 0; i < 4; ++i) {
      input[2*i] = vectors[i].angle;
      input[2*i+1] = 1073741824;
    }
    hal_cordic_buffer_desc_t in = {input, 8};
    hal_cordic_buffer_desc_t out = {output, 8};
    hal_status_t status = HAL_CORDIC_Calculate(mx_cordic_gethandle(), &in, &out, 100);
    for (unsigned i = 0; i < sizeof(vectors)/sizeof(vectors[0]); ++i) {
      int ok = status == HAL_OK && near(output[2*i], vectors[i].cosine) && near(output[2*i+1], vectors[i].sine);
      char line[140];
      snprintf(line, sizeof(line), "[LAB-006] angle=%s cos_q31=%ld sin_q31=%ld status=%u %s\r\n",
               vectors[i].label, (long)output[2*i], (long)output[2*i+1], (unsigned)status, ok ? "PASS" : "FAIL");
      log_line(line);
      passed += (unsigned)ok;
    }
    log_line(passed == 4 ? "[LAB-006] SELFTEST PASS 4/4\r\n" : "[LAB-006] SELFTEST FAIL\r\n");
    HAL_Delay(2000);
  }
}
