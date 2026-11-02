#include "app_led_modes.h"

#include <stdbool.h>
#include <stdint.h>

#include "bsp_button.h"
#include "bsp_led.h"
#include "stm32c5xx.h"
#include "stm32_hal.h"

typedef enum
{
  LED_MODE_BLINK = 0,
  LED_MODE_BREATHE
} led_mode_t;

enum
{
  BLINK_HALF_PERIOD_MS = 1000u,
  BREATHE_STEP_PERIOD_MS = 20u,
  BREATHE_MAX_LEVEL = 100u,
  SOFTWARE_PWM_SLOTS = 100u,
  SOFTWARE_PWM_SLOT_US = 50u
};

static led_mode_t mode;
static bool blink_on;
static uint32_t blink_changed_ms;
static uint32_t breathe_changed_ms;
static uint8_t breathe_level;
static int8_t breathe_direction;
static uint8_t pwm_slot;
static uint32_t pwm_last_cycle;
static uint32_t pwm_slot_cycles;

static void cycle_counter_init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0u;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  pwm_slot_cycles = SystemCoreClock / (1000000u / SOFTWARE_PWM_SLOT_US);
  if (pwm_slot_cycles == 0u)
  {
    pwm_slot_cycles = 1u;
  }
  pwm_last_cycle = DWT->CYCCNT;
}

static void reset_mode_timing(uint32_t now_ms)
{
  blink_on = true;
  blink_changed_ms = now_ms;
  breathe_changed_ms = now_ms;
  breathe_level = 0u;
  breathe_direction = 1;
  pwm_slot = 0u;
  bsp_led_off();
}

static void update_blink(uint32_t now_ms)
{
  if ((uint32_t)(now_ms - blink_changed_ms) >= BLINK_HALF_PERIOD_MS)
  {
    blink_changed_ms += BLINK_HALF_PERIOD_MS;
    blink_on = !blink_on;
  }

  bsp_led_write(blink_on);
}

static void update_breathe_level(uint32_t now_ms)
{
  while ((uint32_t)(now_ms - breathe_changed_ms) >= BREATHE_STEP_PERIOD_MS)
  {
    breathe_changed_ms += BREATHE_STEP_PERIOD_MS;

    if (breathe_direction > 0)
    {
      if (breathe_level < BREATHE_MAX_LEVEL)
      {
        ++breathe_level;
      }
      else
      {
        breathe_direction = -1;
        --breathe_level;
      }
    }
    else if (breathe_level > 0u)
    {
      --breathe_level;
    }
    else
    {
      breathe_direction = 1;
      ++breathe_level;
    }
  }
}

static void update_software_pwm(void)
{
  uint32_t now_cycle = DWT->CYCCNT;

  while ((uint32_t)(now_cycle - pwm_last_cycle) >= pwm_slot_cycles)
  {
    pwm_last_cycle += pwm_slot_cycles;
    ++pwm_slot;
    if (pwm_slot >= SOFTWARE_PWM_SLOTS)
    {
      pwm_slot = 0u;
    }
  }

  bsp_led_write(pwm_slot < breathe_level);
}

void app_led_modes_run(void)
{
  uint32_t now_ms = HAL_GetTick();

  cycle_counter_init();
  bsp_button_init(now_ms);
  mode = LED_MODE_BLINK;
  reset_mode_timing(now_ms);

  for (;;)
  {
    now_ms = HAL_GetTick();

    if (bsp_button_pressed_event(now_ms))
    {
      mode = (mode == LED_MODE_BLINK) ? LED_MODE_BREATHE : LED_MODE_BLINK;
      reset_mode_timing(now_ms);
    }

    if (mode == LED_MODE_BLINK)
    {
      update_blink(now_ms);
    }
    else
    {
      update_breathe_level(now_ms);
      update_software_pwm();
    }
  }
}
