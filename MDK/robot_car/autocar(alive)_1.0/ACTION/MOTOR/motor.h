#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

//暂时用PF0 PF1 PF2 PF3 PF4 PF5 作为输出引脚

#define    PWMA   TIM1->CCR1   //设置定时器比较值
#define    PWMB   TIM1->CCR2
#define    PWMC   TIM1->CCR3
#define    AIN1   PFout(0)
#define    AIN2   PFout(1)
#define    BIN1   PFout(2)
#define    BIN2   PFout(3)
#define    CIN1   PFout(4)
#define    CIN2   PFout(5)

void Motor_Init(void);
void pwminit(u16 arr,u16 psc);

#endif

