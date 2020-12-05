#include "main.h"
//平衡小车49的30转速比的编码电机，精度390，轮子直径58mm,100ms,溢出距离约30米
#define exchange_rate_encoder 0.04672                                                                                                                                       
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

//每1ms中断一次
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
  {
		if(time.taskcount%100==0)
		{
			encoder.v[0]=(read_encoder(3)-encoder.old[0])*exchange_rate_encoder*10;//单位cm/s
			encoder.v[1]=(read_encoder(4)-encoder.old[1])*exchange_rate_encoder*10;
			encoder.v[2]=(read_encoder(5)-encoder.old[2])*exchange_rate_encoder*10;
			encoder.old[0]=read_encoder(3);//脉冲数
			encoder.old[1]=read_encoder(4);
			encoder.old[2]=read_encoder(5);
		}
		time.taskcount+=1;
  }	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


