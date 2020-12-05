#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

//��ʱ��PF0 PF1 PF2 PF3 PF4 PF5 ��Ϊ�������
//PWMΪINT1 IO��ΪINT2
#define    PWM_MAX 3600
#define    PWMA   TIM8->CCR1  //���ö�ʱ���Ƚ�ֵ
#define    AIN1   PEout(10)
#define    AIN2   PEout(11)
#define    PWMB   TIM8->CCR2
#define    BIN1   PEout(12)
#define    BIN2   PEout(13)
#define    PWMC   TIM8->CCR3
#define    CIN1   PEout(14)
#define    CIN2   PEout(15)

void Motor_Init(void);
void pwminit(u16 arr,u16 psc);

#endif

