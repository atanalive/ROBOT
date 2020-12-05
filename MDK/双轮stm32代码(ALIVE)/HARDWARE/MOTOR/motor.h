#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"


#define    PWMA   TIM2->CCR1   //设置定时器比较值
#define    PWMB   TIM2->CCR2
#define    AIN1   PCout(0)
#define    AIN2   PCout(1)
#define    BIN1   PCout(3)
#define    BIN2   PCout(2)

void Motor_Init(void);
void pwminit(u16 arr,u16 psc);

#endif

