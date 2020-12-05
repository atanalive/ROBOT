#ifndef _MAIN_H
#define _MAIN_H

/********系统********/
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "math.h"

/********硬件********/
#include "encoder.h"
#include "tim.h"
#include "motor.h"

/********控制********/
#include "control.h"
#include "moving.h"
#include "pid.h"
#include "task.h"

/*******上位机*******/
#include "DataScope_DP.h"

/********常量*********/
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



