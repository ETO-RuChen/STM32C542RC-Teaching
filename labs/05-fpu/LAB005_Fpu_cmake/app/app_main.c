#include "platform_uart.h"
#include "stm32c542xx.h"

static volatile float sink;

int main(void)
{
  SystemInit();
  platform_uart_init();
  platform_uart_write("[LAB-005] Cortex-M33 FPU benchmark ready\r\n");
  platform_uart_write("[LAB-005] hard-float compiler path, USART2 115200-8-N-1\r\n");

  for (;;) {
    float a = 1.25f;
    float b = 2.5f;
    float c = (a * b) + (a / b);
    sink = c;
    platform_uart_write("[LAB-005] 1.25*2.5+1.25/2.5 x1000 = " );
    platform_uart_write_u32((uint32_t)(sink * 1000.0f));
    platform_uart_write("\r\n");
    for (volatile uint32_t delay = 0; delay < 36000000u; ++delay) { }
  }
}
