#ifndef CAN_LOOPBACK_H
#define CAN_LOOPBACK_H
#include <stdint.h>
int can_loopback_init(void);
int can_loopback_round(uint32_t sequence);
#endif
