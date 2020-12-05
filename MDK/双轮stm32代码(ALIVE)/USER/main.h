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
#include "mpu6050.h"

/********控制********/
#include "control.h"
#include "moving.h"
#include "pid.h"
#include "task.h"
#include "robot.h"

/*******上位机*******/
#include "DataScope_DP.h"

/********常量*********/
#define pi 3.1415926535
#define car_length 10

#define FUNC_VELOCITY 1 //速度环开关
#define FUNC_DIRECTION 0 //差速环开关

void Init_all(void);
void balance_auto_paragram_handle(float set_anger);

#endif



