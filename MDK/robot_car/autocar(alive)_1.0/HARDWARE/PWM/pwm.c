#include "pwm.h"

//TIM1
//PA8  PA9  PA10 �ֱ�Ϊͨ��1��2��3
void pwminit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timeinit; 
	TIM_OCInitTypeDef timeoc;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE); 
	

	//����GPIO
	
	gpio.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOB,&gpio);  
	
	//��ʼ��TIM1
	timeinit.TIM_ClockDivision=0;
	timeinit.TIM_CounterMode=TIM_CounterMode_Up;
	timeinit.TIM_Period=arr;
	timeinit.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM1,&timeinit);
	
	//��ʼ�� pwm����
	timeoc.TIM_OCMode=TIM_OCMode_PWM2;//TIM1_CNT>TIM1_CCRxΪ��Ч�Ƚϲ���
	timeoc.TIM_OCNPolarity=TIM_OCPolarity_High;//�Ƚϼ��Ը�
	timeoc.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM1,&timeoc);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//�߼���ʱ�����������������
	
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���
	TIM_Cmd(TIM1,ENABLE);//ʹ��TIM1
	
	
}




