#include  "hc_sr04.h"
#include  "delay.h"

//暂时选用PF12->TRIG  PF13->ECHO
//TIM6
void hc_sr04_init(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);
	
	/****A6****/
	gpio.GPIO_Pin=GPIO_Pin_12;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&gpio);
	TRIG=0;
	
	/****A7***/
	gpio.GPIO_Pin=GPIO_Pin_13;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOF,&gpio);
	ECHO=0;
	
	/****TIM6****/
	TIM_TimeBaseInitTypeDef tim;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	tim.TIM_Prescaler=71;//计数一次1us
	tim.TIM_CounterMode=TIM_CounterMode_Up;
	tim.TIM_Period=65535;
	tim.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&tim);
	

	TIM_Cmd(TIM6,DISABLE);//先把定时器关了
	
}

u16 get_hc_sr04_hightime(void)
{
	u16 timeout=0;
	u16 time;
	//发送触发信号
	TRIG=1;
	delay_us(15);
  TRIG=0;
	
  TIM1->CNT=0;
	TIM_Cmd(TIM6,DISABLE);
	while(ECHO==0&timeout<20000){delay_us(1);timeout++;};//20ms无响应就跳出中断(防止陷入死循环)
	if(timeout==20000)return 0;
	timeout=0;
	TIM_Cmd(TIM6,ENABLE);
	//设置最远距离4.25m，超过则不测量	(防止陷入死循环)
	while(ECHO==1&&(timeout<25000))
  {
		delay_us(1);
		timeout++;
	}	
	time=TIM6->CNT;
	TIM_Cmd(TIM6,DISABLE);
	if(timeout==25000) return 0;
  return time;
}

u16 get_hc_sr04_distance(void)
{
	u16 time,distance;
	time=0;
	while(time==0)//如果time为0说明测量失败，继续测量
	{
		time=get_hc_sr04_hightime();
	}
	distance=time*340/(2*1e3);//暂时先精确到mm，如果精度不够再做调整
	
	return distance;
}



