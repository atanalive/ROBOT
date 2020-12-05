#ifndef __GRAB_H
#define __GRAB_H
#include "sys.h"

void grab_object(void);
void put_object(void);
u16 steering_calculate_angle(float angle);
void grab_or_put(void);

#endif

