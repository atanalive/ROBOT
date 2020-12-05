#include "main.h"

//1ms是1000hz
//1000=72M(ft)/((arr+1)*(psc+1)) 这里把arr设为3600-1,psc为20-1
//TIM2

//TIM1,TIM8为增强型
//TIM2,TIM3,TIM4,TIM5为通用型
//TIM6T,TIM7为精简型

void timinit(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef timer;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  
  timer.TIM_ClockDivision=TIM_CKD_DIV1;
	timer.TIM_CounterMode=TIM_CounterMode_Up;
	timer.TIM_Period=arr;
	timer.TIM_Prescaler=psc;
	timer.TIM_RepetitionCounter=1;
	
	TIM_TimeBaseInit(TIM2,&timer);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	
	//初始化中断函数
	nvic.NVIC_IRQChannel=TIM2_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	nvic.NVIC_IRQChannelSubPriority=2;//响应优先级   优先级数字越小.优先级越高
	NVIC_Init(&nvic);
	
}

void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
  {
	  encoder.v[0]=read_encoder(3);//计算公式还不知道，到时候拿到编码器数据后再加上去(这里通过计算转化成速度)
		encoder.v[1]=read_encoder(4);
	  encoder.v[2]=read_encoder(5);
		encoder.s[0]+=read_encoder(3);//同上(不过也可以考虑不换算，因为成比例直接用)
		encoder.s[1]+=read_encoder(4);
		encoder.s[2]+=read_encoder(5);
		time.taskcount+=1;
  }	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

