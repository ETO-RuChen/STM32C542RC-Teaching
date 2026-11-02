/**
 * @file mx_button.h
 * @brief Compatibility interface for a CubeMX2 1.1.2 Board Part export defect.
 *
 * CubeMX2 generates mx_button.c for NUCLEO-C542RC but omits this header.
 * Keep this project-owned shim outside generated/ so a new export cannot
 * silently overwrite it. Remove it when the generator emits mx_button.h.
 */
#ifndef LAB001_CUBEMX2_COMPAT_MX_BUTTON_H
#define LAB001_CUBEMX2_COMPAT_MX_BUTTON_H

#include "button.h"
#include "mx_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BUTTON_0                         0u
#define BUTTON_0_GPIO_PORT               HAL_GPIOC
#define BUTTON_0_GPIO_PIN                HAL_GPIO_PIN_13
#define BUTTON_0_GPIO_ACTIVE_STATE       HAL_GPIO_PIN_SET
#define BUTTON_0_EXTI_GETHANDLE()        NULL
#define BUTTON_0_DEBOUNCE_DURATION       20u

button_t *mx_button_0_getobject(void);

#ifdef __cplusplus
}
#endif

#endif /* LAB001_CUBEMX2_COMPAT_MX_BUTTON_H */
