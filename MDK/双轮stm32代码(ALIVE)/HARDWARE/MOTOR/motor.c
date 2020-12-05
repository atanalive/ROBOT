#include "motor.h"

//mg513bp 减速电机:减速比30
//轮子直径58mm
void Motor_Init(void)
{
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
}

//pwm 配置
//TIM2
//PA0  PA1   分别为通道1，2
void pwminit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timeinit; 
	TIM_OCInitTypeDef timeoc;
	
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	

	//配置GPIO
	
	gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	
	GPIO_Init(GPIOA,&gpio);  
	
	//初始化TIM2
	timeinit.TIM_ClockDivision=0;
	timeinit.TIM_CounterMode=TIM_CounterMode_Up;
	timeinit.TIM_Period=arr;
	timeinit.TIM_Prescaler=psc;
	timeinit.TIM_RepetitionCounter=0x00;
	TIM_TimeBaseInit(TIM2,&timeinit);
	
	//初始化 pwm配置
	timeoc.TIM_OCMode=TIM_OCMode_PWM1;//TIM2_CNT<TIM1_CCRx为有效比较部分(向上计数时)
	timeoc.TIM_OCNPolarity=TIM_OCPolarity_High;//比较极性高
	timeoc.TIM_Pulse = 0;       //设置待装入捕获比较寄存器的脉冲值
	timeoc.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM2,&timeoc);//通道1
	TIM_OC2Init(TIM2,&timeoc);//通道2
	
	//TIM_CtrlPWMOutputs(TIM2,ENABLE);//高级定时器输出必须设置这项
	
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);//使能预装载寄存器通道1
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);//使能预装载寄存器通道2
		
	TIM_ARRPreloadConfig(TIM2,ENABLE);//使能TIMx在ARR上的预装载寄存器(影子寄存器)
	TIM_Cmd(TIM2,ENABLE);//使能TIM2
	
	
}




