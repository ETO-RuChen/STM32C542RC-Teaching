#ifndef LAB005_PLATFORM_UART_H
#define LAB005_PLATFORM_UART_H

#include <stdint.h>

void platform_uart_init(void);
void platform_uart_write(const char *text);
void platform_uart_write_u32(uint32_t value);

#endif
