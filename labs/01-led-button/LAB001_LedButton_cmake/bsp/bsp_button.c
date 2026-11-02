#include "bsp_button.h"

#include "mx_gpio_default.h"

enum
{
  BUTTON_SAMPLE_PERIOD_MS = 5u,
  BUTTON_DEBOUNCE_MS = 20u
};

static bool stable_pressed;
static bool candidate_pressed;
static uint32_t candidate_since_ms;
static uint32_t last_sample_ms;

static bool read_pressed(void)
{
  return HAL_GPIO_ReadPin(PC13_PORT, PC13_PIN) == HAL_GPIO_PIN_SET;
}

void bsp_button_init(uint32_t now_ms)
{
  stable_pressed = read_pressed();
  candidate_pressed = stable_pressed;
  candidate_since_ms = now_ms;
  last_sample_ms = now_ms;
}

bool bsp_button_pressed_event(uint32_t now_ms)
{
  bool pressed;

  if ((uint32_t)(now_ms - last_sample_ms) < BUTTON_SAMPLE_PERIOD_MS)
  {
    return false;
  }

  last_sample_ms = now_ms;
  pressed = read_pressed();

  if (pressed != candidate_pressed)
  {
    candidate_pressed = pressed;
    candidate_since_ms = now_ms;
    return false;
  }

  if ((candidate_pressed != stable_pressed) &&
      ((uint32_t)(now_ms - candidate_since_ms) >= BUTTON_DEBOUNCE_MS))
  {
    stable_pressed = candidate_pressed;
    return stable_pressed;
  }

  return false;
}
