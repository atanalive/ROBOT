#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
#include "pid.h"
void control_encoder_init(void);
void motor_velocity_control(void);
void set_motor(u8 i,PID *motor_pid);
u16 abs_num(float a);

#endif


