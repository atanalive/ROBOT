#ifndef __MAIN_H
#define __MAIN_H

/*******硬件类********/
#include "tim.h"
#include "motor.h"
#include "encoder.h"
#include "mpu6050.h"
//#include "hc_sr04.h"
#include "mg995.h"
#include "led.h"
#include "motor.h"
#include "dial_switch.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"

/********控制类********/
#include "task.h"
#include "pid.h"
#include "control.h"
#include "fsm.h"
#include "moving.h"
#include "feedback.h"
#include "path.h"
#include "vision.h"
#include "grab.h"
#include "debug.h"

#define pi 3.1415926535

void init_all(void);
#endif


