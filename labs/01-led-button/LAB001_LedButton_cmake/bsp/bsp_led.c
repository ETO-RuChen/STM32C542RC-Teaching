#include "bsp_led.h"

#include "mx_gpio_default.h"

void bsp_led_write(bool on)
{
  HAL_GPIO_WritePin(PA5_PORT, PA5_PIN, on ? PA5_ACTIVE_STATE : PA5_INACTIVE_STATE);
}

void bsp_led_off(void)
{
  bsp_led_write(false);
}
