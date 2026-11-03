#include "mx_system.h"
#include "mx_usart2.h"
#include "timer_probe.h"
#include <stdio.h>
#include <string.h>

static void log_line(const char *s)
{
  (void)HAL_UART_Transmit(mx_usart2_uart_gethandle(), s, (uint32_t)strlen(s), 1000);
}

int main(void)
{
  if (mx_system_init() != SYSTEM_OK) { for (;;) {} }
  log_line("\r\n[LAB-012] TIM1 internal center-aligned/RCR/software-break probe\r\n");
  for (;;) {
    timer_probe_result r;
    timer_probe_run(&r);
    int ok = r.config_ok && r.directions == 3 &&
             r.updates_rcr0 >= 95 && r.updates_rcr0 <= 105 &&
             r.updates_rcr3 >= 23 && r.updates_rcr3 <= 26 &&
             r.moe_before && !r.moe_after && r.break_flag && r.stayed_off && r.rearmed;
    char line[220];
    snprintf(line, sizeof(line), "[LAB-012] cfg=%lu dirs=%lu updates_100ms: RCR0=%lu RCR3=%lu MOE=%lu->%lu BIF=%lu held=%lu rearm=%lu %s\r\n",
             (unsigned long)r.config_ok, (unsigned long)r.directions,
             (unsigned long)r.updates_rcr0, (unsigned long)r.updates_rcr3,
             (unsigned long)r.moe_before, (unsigned long)r.moe_after,
             (unsigned long)r.break_flag, (unsigned long)r.stayed_off, (unsigned long)r.rearmed,
             ok ? "PASS" : "FAIL");
    log_line(line);
    HAL_Delay(1000);
  }
}
