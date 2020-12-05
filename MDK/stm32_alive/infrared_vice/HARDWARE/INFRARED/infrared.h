#ifndef INFRARED_H
#define INFRARED_H
#include "sys.h"
u8 get_value_infrared(void);
u8 infrared_right_value_handle(void);
u8 infrared_front_value_handle(void);
void led_set(u8 value);
#endif
