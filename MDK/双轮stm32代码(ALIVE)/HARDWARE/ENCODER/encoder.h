#ifndef __ENCODER_
#define __ENCODER_
#include "sys.h"


void encoder_init(void);
void encoder_init_TIM4(void);
void encoder_init_TIM3(void);
u16 read_encoder(u8 TIMX);
void calculate_encoder_velocity(void);



#endif

