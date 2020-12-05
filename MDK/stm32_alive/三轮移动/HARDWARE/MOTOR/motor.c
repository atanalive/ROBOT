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
//TIM8
//PC6  PC7  PC8 分别为通道1，2，3
void pwminit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timeinit; 
	TIM_OCInitTypeDef timeoc;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);//使能时钟
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
	

	//配置GPIO
	
	gpio.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOC,&gpio);  
	
	//初始化TIM8
	timeinit.TIM_ClockDivision=0;
	timeinit.TIM_CounterMode=TIM_CounterMode_Up;
	timeinit.TIM_Period=arr;
	timeinit.TIM_Prescaler=psc;
	timeinit.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM8,&timeinit);
	
	//初始化 pwm配置
	timeoc.TIM_OCMode=TIM_OCMode_PWM1;//TIM8_CNT<TIM1_CCRx为有效比较部分(向上计数时)
	timeoc.TIM_OCNPolarity=TIM_OCPolarity_High;//比较极性高
	timeoc.TIM_Pulse = 0;       //设置待装入捕获比较寄存器的脉冲值
	timeoc.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM8,&timeoc);//通道1
	TIM_OC2Init(TIM8,&timeoc);//通道2
	TIM_OC3Init(TIM8,&timeoc);//通道3
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);//高级定时器输出必须设置这项
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);//使能预装载寄存器通道1
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);//使能预装载寄存器通道2
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);//使能预装载寄存器通道3
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);//使能TIMx在ARR上的预装载寄存器(影子寄存器)
	TIM_Cmd(TIM8,ENABLE);//使能TIM8
	
	
}




