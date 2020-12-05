#include  "mg995.h"

//TIM8  PC6:�������  PC7��ץȡ���
//����Ϊ20ms  arr=200-1    psc=7200-1 ��һ����Ϊ0.1ms
//ͨ��TIM_SetCompare1(TIM1,190)�����ö����ת(ͨ�����Ƹߵ�ƽʱ��)
//(��180�ȽǶȶ��Ϊ��)0.5ms--0��  1.0ms--45��  1.5ms--90��  2.0ms--135��  2.5ms--180��
void mg995_init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef timoc;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_6;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	tim.TIM_Period=arr;
	tim.TIM_Prescaler=psc;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
  tim.TIM_RepetitionCounter=0x00;//��װ�ش���,��TIM1��TIM8��Ч,��������,����Ĭ��,��Ȼpwm�����жϵĴ�����ȷ��
	TIM_TimeBaseInit(TIM8,&tim);
	
	timoc.TIM_OCMode=TIM_OCMode_PWM1;
	timoc.TIM_OutputState=TIM_OutputState_Enable;
	timoc.TIM_Pulse=5;
	timoc.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM8,&timoc);
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);//MOE�����ʹ��
	
	//TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable); 
	//TIM_ARRPreloadConfig(TIM8, ENABLE);//Ϊ����Ӧ����ٶ�,ע�͵�
	
	TIM_Cmd(TIM8,ENABLE);
	
}


