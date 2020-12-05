#ifndef __MG995_H
#define __MG995_H
#include "sys.h"

#define PWMUP    TIM8->CCR1
#define PWMGRAB  TIM8->CCR2

void mg995_init(u16 arr,u16 psc);


#endif

