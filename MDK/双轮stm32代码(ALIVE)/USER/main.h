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
#include "mpu6050.h"

/********����********/
#include "control.h"
#include "moving.h"
#include "pid.h"
#include "task.h"
#include "robot.h"

/*******��λ��*******/
#include "DataScope_DP.h"

/********����*********/
#define pi 3.1415926535
#define car_length 10

#define FUNC_VELOCITY 1 //�ٶȻ�����
#define FUNC_DIRECTION 0 //���ٻ�����

void Init_all(void);
void balance_auto_paragram_handle(float set_anger);

#endif



