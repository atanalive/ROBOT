#include "encoder.h"
#include "usart.h"
#include "pid.h"
#include "robot.h"

//平衡小车49元款的30转速比的编码电机，精度390*4，轮子直径58mm,一个脉冲转换的距离，单位cm
#define exchange_rate_encoder 0.01168   

/*三个编码器分别对应：
TIM4---PB6  PB7  MOTOR_D
TIM3---PA6  PA7  MOTOR_C
TIM3 TIM4 分别对应左，右轮
*/

void encoder_init(void)
{
	encoder_init_TIM3();
	encoder_init_TIM4();
}

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
	///////////////////////////////////////////
	TIM_TimeBaseStructInit(&time);
	///////////////////////////////////////////
	time.TIM_ClockDivision=TIM_CKD_DIV1;
	time.TIM_CounterMode=TIM_CounterMode_Up;
	time.TIM_Period=0xffff;
	time.TIM_Prescaler=0x0;
	TIM_TimeBaseInit(TIM4,&time);
	//设置编码器的模式
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//按照默认值初始化
	TIM_ICStructInit(&timeic);
	timeic.TIM_ICFilter=10;//设置滤波器长度
	TIM_ICInit(TIM4,&timeic);//根据ic数据初始化定时器
	///////////////////////////////////////////
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//使能定时器中断

  TIM_SetCounter(TIM4,0x0fff);//设置TIMx 计数器寄存器值
	///////////////////////////////////////////
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
	///////////////////////////////////////////
	TIM_TimeBaseStructInit(&time);
	///////////////////////////////////////////
	time.TIM_ClockDivision=TIM_CKD_DIV1;
	time.TIM_CounterMode=TIM_CounterMode_Up;
	time.TIM_Period=0xffff;
	time.TIM_Prescaler=0x0;
	TIM_TimeBaseInit(TIM3,&time);
	
	//设置编码器的模式
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	//按照默认值初始化
	TIM_ICStructInit(&timeic);
	timeic.TIM_ICFilter=10;//设置滤波器长度
	TIM_ICInit(TIM3,&timeic);//根据ic数据初始化定时器
	
	///////////////////////////////////////////
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能定时器中断

  TIM_SetCounter(TIM3,0x0fff);//设置TIMx 计数器寄存器值
	///////////////////////////////////////////
	
	TIM_Cmd(TIM3,ENABLE);
	
}



u16 read_encoder(u8 TIMX)
{
	u16 get_encoder;
	switch(TIMX)
  {
		case 4:
			get_encoder=(short)TIM4->CNT;
			TIM4->CNT=0x0fff;
		//printf("TIM4->CNT is %d \r\n",TIM4->CNT);
		break;
		case 3:
		  get_encoder=(short)TIM3->CNT;
			TIM3->CNT=0x0fff;
		//printf("TIM3->CNT is %d \r\n",TIM3->CNT);
		break;
		default:
		  get_encoder=0x0fff;break;
	}
	//printf("get_encoder is %d \r\n",get_encoder);
	return get_encoder;
}

void calculate_encoder_velocity(void)
{
	printf("编码器速度");
	robot.wheel_v[0]=(read_encoder(3)-0x0fff)*exchange_rate_encoder*100;//单位cm/s
	robot.wheel_v[1]=(read_encoder(4)-0x0fff)*exchange_rate_encoder*100;
}
