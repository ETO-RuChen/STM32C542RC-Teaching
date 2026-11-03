#include "timer_probe.h"
#include "stm32_hal.h"
#include "stm32c5xx_ll_tim.h"

static uint32_t measure_updates(uint32_t repetition, uint32_t *directions)
{
  LL_TIM_DisableCounter(TIM1);
  LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP);
  LL_TIM_SetCounter(TIM1, 0);
  LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_CENTER_UP_DOWN);
  LL_TIM_SetRepetitionCounter(TIM1, repetition);
  LL_TIM_GenerateEvent_UPDATE(TIM1);
  LL_TIM_ClearFlag_UPDATE(TIM1);
  uint32_t updates = 0;
  uint32_t start = HAL_GetTick();
  LL_TIM_EnableCounter(TIM1);
  while (HAL_GetTick() - start < 100) {
    *directions |= (TIM1->CR1 & TIM_CR1_DIR) ? 2u : 1u;
    if (LL_TIM_IsActiveFlag_UPDATE(TIM1)) {
      LL_TIM_ClearFlag_UPDATE(TIM1);
      ++updates;
    }
  }
  LL_TIM_DisableCounter(TIM1);
  return updates;
}

void timer_probe_run(timer_probe_result *r)
{
  *r = (timer_probe_result){0};
  r->config_ok = IS_TIM_BREAK_INSTANCE(TIM1) && IS_TIM_REPETITION_COUNTER_INSTANCE(TIM1) &&
                 TIM1->PSC == 47 && TIM1->ARR == 1000 && TIM1->CCR1 == 250 &&
                 (TIM1->CR1 & TIM_CR1_CMS) == TIM_CR1_CMS &&
                 LL_TIM_OC_GetMode(TIM1, LL_TIM_CHANNEL_CH1) == LL_TIM_OCMODE_PWM1;
  r->updates_rcr0 = measure_updates(0, &r->directions);
  r->updates_rcr3 = measure_updates(3, &r->directions);

  /* Observe the internal output gate; no GPIO is mapped in this experiment. */
  LL_TIM_DisableAutomaticOutput(TIM1);
  LL_TIM_ClearFlag_BRK(TIM1);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableAllOutputs(TIM1);
  LL_TIM_EnableCounter(TIM1);
  r->moe_before = (TIM1->BDTR & TIM_BDTR_MOE) != 0;
  LL_TIM_GenerateEvent_BRK(TIM1);
  __DSB();
  uint32_t start = HAL_GetTick();
  while ((TIM1->BDTR & TIM_BDTR_MOE) && HAL_GetTick() - start < 5) { }
  r->moe_after = (TIM1->BDTR & TIM_BDTR_MOE) != 0;
  r->break_flag = LL_TIM_IsActiveFlag_BRK(TIM1) != 0;
  HAL_Delay(20);
  r->stayed_off = (TIM1->BDTR & TIM_BDTR_MOE) == 0;
  LL_TIM_ClearFlag_BRK(TIM1);
  LL_TIM_EnableAllOutputs(TIM1);
  start = HAL_GetTick();
  while (!(TIM1->BDTR & TIM_BDTR_MOE) && HAL_GetTick() - start < 5) { }
  r->rearmed = (TIM1->BDTR & TIM_BDTR_MOE) != 0;
  LL_TIM_DisableAllOutputs(TIM1);
  LL_TIM_DisableCounter(TIM1);
}
