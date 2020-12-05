#ifndef __CONTROL_H
#define __CONTROL_H
#include "main.h"
#include "pid.h"
void pid_all_init(void);
void motor_velocity_control(void);
void set_motor(u8 i,float output);
u16 abs_num(float a);
float output_limit(float output,float limit);

#endif


