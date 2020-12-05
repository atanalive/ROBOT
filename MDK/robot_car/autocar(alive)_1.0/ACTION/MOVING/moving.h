#ifndef __MOVING_H
#define __MOVING_H
#include "sys.h"



struct MOVING_CONTROL
{
	float V_max;//速度最大值控制
	float V_low;//低速行驶速度
	u8 V_low_mode;//低速行驶控制
	float s_slow_down;//减速距离
	float s_speed_up;//加速距离
	float a1;//加速的加速度
	float a2;//减速的加速度
};

//参数设置
extern struct MOVING_CONTROL moving;

float float_abs(float value);
void coordinates_to_wheelvelocity_changeover(void);
float moving_line_velocity_planning_trapezoid(float b,float s);
void turn_around(void);
void go_straight_or_turn_around(void);
void moving_line_track(void);
void whirling_180_angle(void);


#endif



