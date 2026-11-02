#include "app_timer_pwm.h"
#include <stdio.h>
#include <string.h>
#include "mx_tim2.h"
#include "mx_usart2.h"
#include "stm32_hal.h"

static hal_tim_handle_t *s_tim;
static hal_uart_handle_t *s_uart;

static void configure_pwm_pin(void)
{
  hal_gpio_config_t gpio_config;

  HAL_RCC_GPIOA_EnableClock();
  gpio_config.mode = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull = HAL_GPIO_PULL_NO;
  gpio_config.speed = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate = HAL_GPIO_AF_1;
  (void)HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_5, &gpio_config);
}

static void write_uart(const char *text)
{
  (void)HAL_UART_Transmit(s_uart, text, (uint32_t)strlen(text), 1000U);
}

int app_timer_pwm_init(void)
{
  s_tim = mx_tim2_gethandle();
  s_uart = mx_usart2_uart_gethandle();
  if ((s_tim == NULL) || (s_uart == NULL))
  {
    return -1;
  }

  /* CubeMX2 2.1.0 does not emit TIM2_CH1 GPIO setup for this board
     configuration, so the project-owned compatibility layer supplies it. */
  configure_pwm_pin();

  if (HAL_TIM_Start(s_tim) != HAL_OK)
  {
    return -1;
  }
  if (HAL_TIM_OC_StartChannel(s_tim, HAL_TIM_CHANNEL_1) != HAL_OK)
  {
    return -1;
  }

  write_uart("\r\n[LAB-003] TIM2 hardware PWM ready\r\n");
  write_uart("[LAB-003] PA5 TIM2_CH1, 144 Hz, ARR=999, PSC=143\r\n");
  return 0;
}

void app_timer_pwm_run(void)
{
  uint32_t next_report = HAL_GetTick();
  uint32_t duty = 0U;
  int32_t step = 20;
  char line[96];

  for (;;)
  {
    const uint32_t now = HAL_GetTick();
    if ((int32_t)(now - next_report) >= 0)
    {
      const int length = snprintf(line, sizeof(line), "[LAB-003] tick_ms=%lu duty=%lu/1000\r\n",
                                  (unsigned long)now, (unsigned long)duty);
      if (length > 0)
      {
        (void)HAL_UART_Transmit(s_uart, line, (uint32_t)length, 1000U);
      }
      (void)HAL_TIM_OC_SetCompareUnitPulse(s_tim, HAL_TIM_OC_COMPARE_UNIT_1, duty);
      duty = (uint32_t)((int32_t)duty + step);
      if (duty >= 1000U)
      {
        duty = 1000U;
        step = -20;
      }
      else if (duty == 0U)
      {
        step = 20;
      }
      next_report += 20U;
    }
  }
}
