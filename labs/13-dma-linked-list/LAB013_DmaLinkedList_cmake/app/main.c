#include "mx_system.h"
#include "mx_usart2.h"
#include "dma_queue_probe.h"
#include <stdio.h>
#include <string.h>

static void log_text(const char *s)
{
  (void)HAL_UART_Transmit(mx_usart2_uart_gethandle(), s, (uint32_t)strlen(s), 1000);
}
int main(void)
{
  if (mx_system_init() != SYSTEM_OK) { for (;;) {} }
  log_text("\r\n[LAB-013] LPDMA1_CH0 three-node linear memory queue\r\n");
  int init = dma_queue_probe_init();
  if (init) {
    char line[60];
    snprintf(line, sizeof(line), "[LAB-013] init FAIL rc=%d\r\n", init);
    log_text(line);
    for (;;) {}
  }
  for (uint32_t sequence=0;; ++sequence) {
    dma_queue_result r;
    int rc = dma_queue_probe_run(sequence, &r);
    char line[150];
    snprintf(line, sizeof(line), "[LAB-013] seq=%lu nodes=3 words=%lu/96 guards=%lu/6 callbacks=%lu rc=%d %s\r\n",
             (unsigned long)sequence, (unsigned long)r.words_ok, (unsigned long)r.guards_ok,
             (unsigned long)r.completions, rc, rc == 0 ? "PASS" : "FAIL");
    log_text(line);
    if (rc) { for (;;) {} }
    HAL_Delay(1000);
  }
}
