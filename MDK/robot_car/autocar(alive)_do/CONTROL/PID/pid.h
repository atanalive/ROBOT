#ifndef __PID_H
#define __PID_H
#include "sys.h"

typedef struct pidparameter
{
	float kp;
	float ki;
	float kd;
	float last_e;//��һ�ε����
	float e;//�������
	float accumulate_e;//�ۼ����
	float pwm;//Ҫ�����pwm�Ƚ�ֵ
	float output;//�����ֵ
	float output_limit;//����޷�		
	float integral_limit;//�����޷�
	float deadzone;//����
	float firstflag;//�����һ�ε��������
	float delta_e;
}PID;

void pidinit(PID *pid,float kp,float ki,float kd,float outmax,float integralmax,float deadzone);

float limit_value(float num,float limit);

void pid_position_calculate(PID *pid,int currentpoint,int nextpoint);
void pid_position_calculate_error(PID *pid,float error);

#endif


