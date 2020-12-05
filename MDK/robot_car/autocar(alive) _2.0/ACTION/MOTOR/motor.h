#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

//��ʱ��PF0 PF1 PF2 PF3 PF4 PF5 ��Ϊ�������

#define    PWMA   TIM1->CCR1   //���ö�ʱ���Ƚ�ֵ
#define    PWMB   TIM1->CCR2
#define    PWMC   TIM1->CCR3
#define    AIN1   PEout(10)
#define    AIN2   PEout(11)
#define    BIN1   PEout(12)
#define    BIN2   PEout(13)
#define    CIN1   PEout(14)
#define    CIN2   PEout(15)

void Motor_Init(void);
void pwminit(u16 arr,u16 psc);

#endif

