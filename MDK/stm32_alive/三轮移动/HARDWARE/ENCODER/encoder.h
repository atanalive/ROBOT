#ifndef __ENCODER_
#define __ENCODER_
#include "sys.h"

struct ENCODER
{	
	float  v[3];  //三个轮子速度
	//int  s[3];  //三个轮子的路程
	//u16  old[3];//旧数据(脉冲)
	float target_velocity[3];
};
extern struct ENCODER encoder;

//#define changeover_encoder 360  //每米对应的编码器脉冲值(计数值除以四) (假设为360)

void encoder_init(void);
void encoder_init_TIM4(void);
void encoder_init_TIM3(void);
void encoder_init_TIM5(void);
u16 read_encoder(u8 TIMX);
void calculate_encoder_velocity(void);



#endif

