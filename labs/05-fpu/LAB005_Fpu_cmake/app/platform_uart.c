#include "platform_uart.h"
#include "stm32c542xx.h"

/* USART2 is routed to the NUCLEO ST-LINK VCP on PA2/PA3. */
#define UART_BAUD 115200u

static void uart_putc(char c)
{
  while ((USART2->ISR & USART_ISR_TXE_TXFNF) == 0u) { }
  USART2->TDR = (uint8_t)c;
}

void platform_uart_init(void)
{
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->APB1LENR |= RCC_APB1LENR_USART2EN;
  GPIOA->MODER = (GPIOA->MODER & ~((3u << (2u * 2u)) | (3u << (3u * 2u)))) |
                 (2u << (2u * 2u)) | (2u << (3u * 2u));
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~((0xFu << (2u * 4u)) | (0xFu << (3u * 4u)))) |
                  (7u << (2u * 4u)) | (7u << (3u * 4u));
  USART2->CR1 = 0u;
  USART2->BRR = 48000000u / UART_BAUD;
  USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void platform_uart_write(const char *text)
{
  while (*text != '\0') { uart_putc(*text++); }
}

void platform_uart_write_u32(uint32_t value)
{
  char buffer[11];
  unsigned index = sizeof(buffer);
  buffer[--index] = '\0';
  do { buffer[--index] = (char)('0' + (value % 10u)); value /= 10u; } while (value != 0u);
  platform_uart_write(&buffer[index]);
}
