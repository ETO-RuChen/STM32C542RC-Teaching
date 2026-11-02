 #include "app_entry.h"
 #include "app_uart_logging.h"
 #include "mx_system.h"

 system_status_t post_system_init_hook(void)
 {
   return app_uart_logging_init() == 0 ? SYSTEM_OK : SYSTEM_POSTSYSTEM_ERROR;
 }

 void app_run(void)
 {
   app_uart_logging_run();
 }
