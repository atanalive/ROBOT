#include "main.h"
//ƽ��С��49��30ת�ٱȵı�����������390������ֱ��58mm,100ms,�������Լ30��
#define exchange_rate_encoder 0.04672                                                                                                                                       
//1ms��1000hz
//1000=72M(ft)/((arr+1)*(psc+1)) �����arr��Ϊ3600-1,pscΪ20-1
//TIM2

//TIM1,TIM8Ϊ��ǿ��
//TIM2,TIM3,TIM4,TIM5Ϊͨ����
//TIM6T,TIM7Ϊ������

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
	
	//��ʼ���жϺ���
	nvic.NVIC_IRQChannel=TIM2_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	nvic.NVIC_IRQChannelSubPriority=2;//��Ӧ���ȼ�   ���ȼ�����ԽС.���ȼ�Խ��
	NVIC_Init(&nvic);
	}

//ÿ1ms�ж�һ��
void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
  {
		if(time.taskcount%100==0)
		{
			encoder.v[0]=(read_encoder(3)-encoder.old[0])*exchange_rate_encoder*10;//��λcm/s
			encoder.v[1]=(read_encoder(4)-encoder.old[1])*exchange_rate_encoder*10;
			encoder.v[2]=(read_encoder(5)-encoder.old[2])*exchange_rate_encoder*10;
			encoder.old[0]=read_encoder(3);//������
			encoder.old[1]=read_encoder(4);
			encoder.old[2]=read_encoder(5);
		}
		time.taskcount+=1;
  }	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


