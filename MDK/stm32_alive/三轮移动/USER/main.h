#ifndef _MAIN_H
#define _MAIN_H

/********ϵͳ********/
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "math.h"

/********Ӳ��********/
#include "encoder.h"
#include "tim.h"
#include "motor.h"

/********����********/
#include "control.h"
#include "moving.h"
#include "pid.h"
#include "task.h"

/*******��λ��*******/
#include "DataScope_DP.h"

/********����*********/
#define pi 3.1415926535
#define car_length 10


struct ROBOT
{
	float initial_yaw;
	float yaw;
	float target_Vx;
	float target_Vy;
	float target_w;
};

extern struct ROBOT ROBOT_NOW;

#endif



