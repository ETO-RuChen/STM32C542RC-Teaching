#ifndef LAB001_BSP_BUTTON_H
#define LAB001_BSP_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

void bsp_button_init(uint32_t now_ms);
bool bsp_button_pressed_event(uint32_t now_ms);

#endif /* LAB001_BSP_BUTTON_H */
