#include "mx_system.h"
#include "mx_usart2.h"
#include "aes_probe.h"
#include <stdio.h>
#include <string.h>

static void log_text(const char *s)
{
  (void)HAL_UART_Transmit(mx_usart2_uart_gethandle(), s, (uint32_t)strlen(s), 1000);
}
int main(void)
{
  if (mx_system_init() != SYSTEM_OK) { for (;;) {} }
  log_text("\r\n[LAB-008/AES] AES-128 ECB NIST public-vector test, 4 blocks\r\n");
  for (unsigned sequence=0;; ++sequence) {
    aes_probe_result r;
    int rc = aes_probe_run(&r);
    char line[180];
    snprintf(line, sizeof(line), "[LAB-008/AES] seq=%u enc=%lu/4 dec=%lu/4 wrong_key_diff=%lu guards=%lu/6 rc=%d %s\r\n",
             sequence, (unsigned long)r.encrypt_blocks, (unsigned long)r.decrypt_blocks,
             (unsigned long)r.wrong_key_diff, (unsigned long)r.guards, rc, rc == 0 ? "PASS" : "FAIL");
    log_text(line);
    snprintf(line, sizeof(line), "[LAB-008/AES] cipher0=%08lx%08lx%08lx%08lx\r\n",
             (unsigned long)r.first_cipher[0], (unsigned long)r.first_cipher[1],
             (unsigned long)r.first_cipher[2], (unsigned long)r.first_cipher[3]);
    log_text(line);
    if (rc) { for (;;) {} }
    HAL_Delay(1000);
  }
}
