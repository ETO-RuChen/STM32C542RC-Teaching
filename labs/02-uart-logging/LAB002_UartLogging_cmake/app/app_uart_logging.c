 #include "app_uart_logging.h"
 #include <stdio.h>
 #include <string.h>
 #include "mx_usart2.h"
 #include "stm32_hal.h"

 static hal_uart_handle_t *s_uart;

 static void app_uart_write(const char *text)
 {
   (void)HAL_UART_Transmit(s_uart, text, (uint32_t)strlen(text), 1000U);
 }

 int app_uart_logging_init(void)
 {
   s_uart = mx_usart2_uart_gethandle();
   if (s_uart == NULL)
   {
     return -1;
   }
   app_uart_write("\r\n[LAB-002] STM32C542RC USART2 logging ready\r\n");
   app_uart_write("[LAB-002] 115200 8-N-1, ST-LINK VCP, PA2/PA3\r\n");
   return 0;
 }

 void app_uart_logging_run(void)
 {
   uint32_t sequence = 0U;
   uint32_t next_report = HAL_GetTick();
   char line[96];

   for (;;)
   {
     const uint32_t now = HAL_GetTick();
     if ((int32_t)(now - next_report) >= 0)
     {
       const int length = snprintf(line, sizeof(line), "[LAB-002] seq=%lu tick_ms=%lu\r\n",
                                   (unsigned long)sequence++, (unsigned long)now);
       if (length > 0)
       {
         (void)HAL_UART_Transmit(s_uart, line, (uint32_t)length, 1000U);
       }
       next_report += 1000U;
     }
   }
 }
