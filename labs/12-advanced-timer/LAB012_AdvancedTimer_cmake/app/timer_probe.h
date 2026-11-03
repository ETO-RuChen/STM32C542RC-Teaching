#ifndef TIMER_PROBE_H
#define TIMER_PROBE_H
#include <stdint.h>
typedef struct {
  uint32_t updates_rcr0;
  uint32_t updates_rcr3;
  uint32_t directions;
  uint32_t moe_before;
  uint32_t moe_after;
  uint32_t break_flag;
  uint32_t stayed_off;
  uint32_t rearmed;
  uint32_t config_ok;
} timer_probe_result;
void timer_probe_run(timer_probe_result *result);
#endif
