#include "pwm.h"

//TIM1
//PA8  PA9  PA10 分别为通道1，2，3
void pwminit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_TimeBaseInitTypeDef timeinit; 
	TIM_OCInitTypeDef timeoc;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE); 
	

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
	TIM_TimeBaseInit(TIM1,&timeinit);
	
	//初始化 pwm配置
	timeoc.TIM_OCMode=TIM_OCMode_PWM2;//TIM1_CNT>TIM1_CCRx为有效比较部分
	timeoc.TIM_OCNPolarity=TIM_OCPolarity_High;//比较极性高
	timeoc.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC2Init(TIM1,&timeoc);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//高级定时器输出必须设置这项
	
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);//使能预装载寄存器
	TIM_Cmd(TIM1,ENABLE);//使能TIM1
	
	
}




