#ifndef __ENCODER_
#define __ENCODER_
#include "sys.h"

struct ENCODER
{	
	float  v[3];  //���������ٶ�
	//int  s[3];  //�������ӵ�·��
	//u16  old[3];//������(����)
	float target_velocity[3];
};
extern struct ENCODER encoder;

//#define changeover_encoder 360  //ÿ�׶�Ӧ�ı���������ֵ(����ֵ������) (����Ϊ360)

void encoder_init(void);
void encoder_init_TIM4(void);
void encoder_init_TIM3(void);
void encoder_init_TIM5(void);
u16 read_encoder(u8 TIMX);
void calculate_encoder_velocity(void);



#endif

