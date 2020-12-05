#ifndef __PID_H
#define __PID_H
#include "sys.h"

typedef struct pidparameter
{
	float kp;
	float ki;
	float kd;
	float last_e;//上一次的误差
	float e;//本次误差
	float accumulate_e;//累加误差
	float pwm;//要输出的pwm比较值
	float output;//输出的值
	float output_limit;//输出限幅		
	float integral_limit;//积分限幅
	float deadzone;//死区
	float firstflag;//处理第一次的特殊情况
	float delta_e;
}PID;

void pidinit(PID *pid,float kp,float ki,float kd,float outmax,float integralmax,float deadzone);

float limit_value(float num,float limit);

void pid_position_calculate(PID *pid,int currentpoint,int nextpoint);
void pid_position_calculate_error(PID *pid,float error);

#endif


