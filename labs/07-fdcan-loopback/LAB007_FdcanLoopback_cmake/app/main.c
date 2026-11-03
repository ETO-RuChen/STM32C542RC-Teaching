#include "mx_system.h"
#include "mx_usart2.h"
#include "can_loopback.h"
#include <stdio.h>
#include <string.h>

static void log_text(const char *s)
{
  (void)HAL_UART_Transmit(mx_usart2_uart_gethandle(), s, (uint32_t)strlen(s), 1000);
}

int main(void)
{
  if (mx_system_init() != SYSTEM_OK) { for (;;) {} }
  log_text("\r\n[LAB-007] FDCAN1 internal loopback, classic 500kbps, filter=0x123/0x7FF\r\n");
  int init = can_loopback_init();
  if (init) {
    char line[60];
    snprintf(line, sizeof(line), "[LAB-007] init FAIL rc=%d\r\n", init);
    log_text(line);
    for (;;) {}
  }
  for (uint32_t sequence = 0;; ++sequence) {
    int rc = can_loopback_round(sequence);
    char line[140];
    snprintf(line, sizeof(line), "[LAB-007] seq=%lu accept_0x123+payload reject_0x321 rc=%d %s\r\n",
             (unsigned long)sequence, rc, rc == 0 ? "PASS" : "FAIL");
    log_text(line);
    if (rc) { for (;;) {} } /* Preserve the failed state for debugger inspection. */
    HAL_Delay(1000);
  }
}
