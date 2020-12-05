#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"

void control_encoder_init(void);
void motor_velocity_control(void);
void wheel_journey_control(u8 i);
void set_motor(u8 i,PID moto_pid);
u16 abs_num(float a);

#endif


