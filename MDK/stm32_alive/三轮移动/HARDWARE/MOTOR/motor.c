#include "motor.h"

//mg513bp ���ٵ��:���ٱ�30
//����ֱ��58mm
void Motor_Init(void)
{
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&gpio);
	
}

//pwm ����
//TIM8
//PC6  PC7  PC8 �ֱ�Ϊͨ��1��2��3
void pwminit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timeinit; 
	TIM_OCInitTypeDef timeoc;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//ʹ��ʱ��
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
	

	//����GPIO
	
	gpio.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOC,&gpio);  
	
	//��ʼ��TIM8
	timeinit.TIM_ClockDivision=0;
	timeinit.TIM_CounterMode=TIM_CounterMode_Up;
	timeinit.TIM_Period=arr;
	timeinit.TIM_Prescaler=psc;
	timeinit.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM8,&timeinit);
	
	//��ʼ�� pwm����
	timeoc.TIM_OCMode=TIM_OCMode_PWM1;//TIM8_CNT<TIM1_CCRxΪ��Ч�Ƚϲ���(���ϼ���ʱ)
	timeoc.TIM_OCNPolarity=TIM_OCPolarity_High;//�Ƚϼ��Ը�
	timeoc.TIM_Pulse = 0;       //���ô�װ�벶��ȽϼĴ���������ֵ
	timeoc.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM8,&timeoc);//ͨ��1
	TIM_OC2Init(TIM8,&timeoc);//ͨ��2
	TIM_OC3Init(TIM8,&timeoc);//ͨ��3
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);//�߼���ʱ�����������������
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���ͨ��1
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���ͨ��2
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���ͨ��3
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���(Ӱ�ӼĴ���)
	TIM_Cmd(TIM8,ENABLE);//ʹ��TIM8
	
	
}




