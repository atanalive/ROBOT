#include "motor.h"

//mg513bp 减速电机:减速比30
//轮子直径58mm
void Motor_Init(void)
{
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&gpio);
	
}

//pwm 配置
//TIM1
//PA8  PA9  PA10 分别为通道1，2，3
void pwminit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timeinit; 
	TIM_OCInitTypeDef timeoc;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	

	//配置GPIO
	
	gpio.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOB,&gpio);  
	
	//初始化TIM1
	timeinit.TIM_ClockDivision=0;
	timeinit.TIM_CounterMode=TIM_CounterMode_Up;
	timeinit.TIM_Period=arr;
	timeinit.TIM_Prescaler=psc;
	timeinit.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM1,&timeinit);
	
	//初始化 pwm配置
	timeoc.TIM_OCMode=TIM_OCMode_PWM1;//TIM1_CNT<TIM1_CCRx为有效比较部分(向上计数时)
	timeoc.TIM_OCNPolarity=TIM_OCPolarity_Low;//比较极性高
	timeoc.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM1,&timeoc);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//高级定时器输出必须设置这项
	
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能预装载寄存器
	TIM_ARRPreloadConfig(TIM1,ENABLE);//使能TIMx在ARR上的预装载寄存器(影子寄存器)
	TIM_Cmd(TIM1,ENABLE);//使能TIM1
	
	
}




