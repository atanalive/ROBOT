#include "main.h"
#include "tim.h"
#include "usart.h"
  
//ƽ��С��49��30ת�ٱȵı�����������390������ֱ��58mm,100ms,�������Լ30��
#define exchange_rate_encoder 0.04672    		 
//1ms��1000hz
//1000=72M(ft)/((arr+1)*(psc+1)) �����arr��Ϊ3600-1,pscΪ20-1
//TIM1

//TIM1,TIM8Ϊ��ǿ��
//TIM2,TIM3,TIM4,TIM5Ϊͨ����
//TIM6T,TIM7Ϊ������

void timinit(u16 arr,u16 psc)
{ 
	TIM_TimeBaseInitTypeDef timer;
	NVIC_InitTypeDef nvic;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
  
  timer.TIM_ClockDivision=TIM_CKD_DIV1;
	timer.TIM_CounterMode=TIM_CounterMode_Up;
	timer.TIM_Period=arr;
	timer.TIM_Prescaler=psc;
	timer.TIM_RepetitionCounter=1;
	
	TIM_TimeBaseInit(TIM1,&timer);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	
	//��ʼ���жϺ���
	nvic.NVIC_IRQChannel=TIM1_UP_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	nvic.NVIC_IRQChannelSubPriority=2;//��Ӧ���ȼ�   ���ȼ�����ԽС.���ȼ�Խ��
	NVIC_Init(&nvic);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM1,ENABLE); //ʹ�ܶ�ʱ��2
	
	
	}

//ÿ1ms�ж�һ��
void TIM1_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)==SET)
  {
		
		time.taskcount+=1;
	}	
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}


