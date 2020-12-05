#ifndef __ENCODER_
#define __ENCODER_
#include "sys.h"

struct ENCODER
{	
	int  v[3];  //���������ٶ�
	int  s[3];  //�������ӵ�·��
	int target_velocity[3];
  int target_location[3];
};
extern struct ENCODER encoder;

#define changeover_encoder 360  //ÿ�׶�Ӧ�ı���������ֵ(����ֵ������) (����Ϊ360)

void encoder_init_TIM4(void);
void encoder_init_TIM3(void);
void encoder_init_TIM5(void);
int read_encoder(u8 TIMX);



#endif

