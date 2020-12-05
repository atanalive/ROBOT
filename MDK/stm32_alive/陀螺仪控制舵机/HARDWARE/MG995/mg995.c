#include  "mg995.h"

//TIM8  PA8:升降舵机  PA11：抓取舵机
//周期为20ms  arr=20000-1    psc=72-1 计100次数为0.1ms
//通过TIM_SetCompare1(TIM1,190)函数让舵机旋转(通过控制高电平时间)
//(以180度角度舵机为例)0.5ms--0度  1.0ms--45度  1.5ms--90度  2.0ms--135度  2.5ms--180度
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
	tim.TIM_RepetitionCounter=0x00;//重装载次数,仅TIM1和TIM8有效,必须设置,不能默认,不然pwm更新中断的次数不确定
	TIM_TimeBaseInit(TIM1,&tim);
	
	timoc.TIM_OCMode=TIM_OCMode_PWM1;
	timoc.TIM_OutputState=TIM_OutputState_Enable;
	timoc.TIM_Pulse=0;
	timoc.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM1,&timoc);
	TIM_OC2Init(TIM1,&timoc);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//MOE主输出使能
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能预装载寄存器通道1
	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);//使能预装载寄存器通道2
	//TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(TIM1, ENABLE);//为了响应提高速度,注释掉
	TIM_Cmd(TIM1,ENABLE);
	
}


