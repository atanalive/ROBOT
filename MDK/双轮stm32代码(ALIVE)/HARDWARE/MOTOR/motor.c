#include "motor.h"

//mg513bp ���ٵ��:���ٱ�30
//����ֱ��58mm
void Motor_Init(void)
{
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
}

//pwm ����
//TIM2
//PA0  PA1   �ֱ�Ϊͨ��1��2
void pwminit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timeinit; 
	TIM_OCInitTypeDef timeoc;
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	

	//����GPIO
	
	gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOA,&gpio);  
	
	//��ʼ��TIM2
	timeinit.TIM_ClockDivision=0;
	timeinit.TIM_CounterMode=TIM_CounterMode_Up;
	timeinit.TIM_Period=arr;
	timeinit.TIM_Prescaler=psc;
	timeinit.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM2,&timeinit);
	
	//��ʼ�� pwm����
	timeoc.TIM_OCMode=TIM_OCMode_PWM1;//TIM2_CNT<TIM1_CCRxΪ��Ч�Ƚϲ���(���ϼ���ʱ)
	timeoc.TIM_OCNPolarity=TIM_OCPolarity_High;//�Ƚϼ��Ը�
	timeoc.TIM_Pulse = 0;       //���ô�װ�벶��ȽϼĴ���������ֵ
	timeoc.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM2,&timeoc);//ͨ��1
	TIM_OC2Init(TIM2,&timeoc);//ͨ��2
	
	//TIM_CtrlPWMOutputs(TIM2,ENABLE);//�߼���ʱ�����������������
	
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���ͨ��1
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);//ʹ��Ԥװ�ؼĴ���ͨ��2
		
	TIM_ARRPreloadConfig(TIM2,ENABLE);//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���(Ӱ�ӼĴ���)
	TIM_Cmd(TIM2,ENABLE);//ʹ��TIM2
	
	
}




