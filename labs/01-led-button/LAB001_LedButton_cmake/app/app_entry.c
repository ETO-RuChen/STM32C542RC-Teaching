#include "app_led_modes.h"
#include "mx_system.h"

/**
 * CubeMX2 calls this strong implementation after all generated system and
 * peripheral initialization has succeeded. The teaching application owns the
 * foreground loop from this point onward.
 */
system_status_t post_system_init_hook(void)
{
  app_led_modes_run();

  /* app_led_modes_run() is intentionally non-returning. */
  return SYSTEM_OK;
}
