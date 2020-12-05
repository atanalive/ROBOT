#include  "mg995.h"

//TIM8  PA8:�������  PA11��ץȡ���
//����Ϊ20ms  arr=20000-1    psc=72-1 ��100����Ϊ0.1ms
//ͨ��TIM_SetCompare1(TIM1,190)�����ö����ת(ͨ�����Ƹߵ�ƽʱ��)
//(��180�ȽǶȶ��Ϊ��)0.5ms--0��  1.0ms--45��  1.5ms--90��  2.0ms--135��  2.5ms--180��
void mg995_init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef tim;
	TIM_OCInitTypeDef timoc;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	TIM_TimeBaseStructInit(&tim);
	
	tim.TIM_Period=arr;
	tim.TIM_Prescaler=psc;
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	tim.TIM_RepetitionCounter=0x00;//��װ�ش���,��TIM1��TIM8��Ч,��������,����Ĭ��,��Ȼpwm�����жϵĴ�����ȷ��
	TIM_TimeBaseInit(TIM1,&tim);
	
	timoc.TIM_OCMode=TIM_OCMode_PWM1;
	timoc.TIM_OutputState=TIM_OutputState_Enable;
	timoc.TIM_Pulse=0;
	timoc.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM1,&timoc);
	TIM_OC2Init(TIM1,&timoc);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//MOE�����ʹ��
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���ͨ��1
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���ͨ��2
	//TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(TIM1, ENABLE);//Ϊ����Ӧ����ٶ�,ע�͵�
	TIM_Cmd(TIM1,ENABLE);
	
}


