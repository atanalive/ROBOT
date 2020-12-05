#include "encoder.h"

/*三个编码器分别对应：
TIM4---PB6  PB7
TIM3---PA6  PA7
TIM5---PA0  PA1
TIM3 TIM4 TIM5 分别对应A B C轮(A:与y轴负方向重合，后轮，B:第一象限，右上角轮，C:第二象限：左上角轮)
每个定时器只有ch1，ch2通道可以用作编码器
*/

struct ENCODER encoder;//储存编码器得到的值

void encoder_init_TIM4(void)
{
	TIM_TimeBaseInitTypeDef time;
	TIM_ICInitTypeDef timeic;
	GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
	time.TIM_ClockDivision=TIM_CKD_DIV1;
	time.TIM_CounterMode=TIM_CounterMode_Up;
	time.TIM_Period=0xFFFF;
	time.TIM_Prescaler=0x0;
	TIM_TimeBaseInit(TIM4,&time);
	//设置编码器的模式
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//按照默认值初始化
	TIM_ICStructInit(&timeic);
	timeic.TIM_ICFilter=10;//设置滤波器长度
	TIM_ICInit(TIM4,&timeic);//根据ic数据初始化定时器
	
	TIM_Cmd(TIM4,ENABLE);
	
	
}

void encoder_init_TIM3(void)
{
	TIM_TimeBaseInitTypeDef time;
	TIM_ICInitTypeDef timeic;
	GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	time.TIM_ClockDivision=TIM_CKD_DIV1;
	time.TIM_CounterMode=TIM_CounterMode_Up;
	time.TIM_Period=0xFFFF;
	time.TIM_Prescaler=0x0;
	TIM_TimeBaseInit(TIM3,&time);
	//设置编码器的模式
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//按照默认值初始化
	TIM_ICStructInit(&timeic);
	timeic.TIM_ICFilter=10;//设置滤波器长度
	TIM_ICInit(TIM3,&timeic);//根据ic数据初始化定时器
	
	TIM_Cmd(TIM3,ENABLE);
	
	
}

void encoder_init_TIM5(void)
{
	TIM_TimeBaseInitTypeDef time;
	TIM_ICInitTypeDef timeic;
	GPIO_InitTypeDef gpio;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	time.TIM_ClockDivision=TIM_CKD_DIV1;
	time.TIM_CounterMode=TIM_CounterMode_Up;
	time.TIM_Period=0xFFFF;
	time.TIM_Prescaler=0x0;
	TIM_TimeBaseInit(TIM5,&time);
	//设置编码器的模式
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//按照默认值初始化
	TIM_ICStructInit(&timeic);
	timeic.TIM_ICFilter=10;//设置滤波器长度
	TIM_ICInit(TIM5,&timeic);//根据ic数据初始化定时器
	
	TIM_Cmd(TIM5,ENABLE);
	
}

int read_encoder(u8 TIMX)
{
	int encoder;
	switch(TIMX)
  {
		case 4:
			encoder=TIM4->CNT;break;
		case 3:
		  encoder=TIM3->CNT;break;
		case 5:
			encoder=TIM5->CNT;break;
		default:
		  encoder=0;
	}
	return encoder;
}


