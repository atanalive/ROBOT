#include "raspberry.h"
#include "usart.h"

//重映射引脚PD5 PD6
void usart2_init(u32 bound)
{
	//定义结构体
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart2;
	NVIC_InitTypeDef nvic;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
	
	//GPIO初始化
	//USART2 TX PD5
	gpio.GPIO_Pin=GPIO_Pin_5;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&gpio);
	
	//USART2 RX PD6
	gpio.GPIO_Pin=GPIO_Pin_6;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&gpio);
	
	//usart2 NVIC 配置
	nvic.NVIC_IRQChannel=USART2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=3;
	nvic.NVIC_IRQChannelSubPriority=3;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	
	//usart2 初始化
	usart2.USART_BaudRate=bound;
	usart2.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	usart2.USART_WordLength=USART_WordLength_8b;
	usart2.USART_StopBits=USART_StopBits_1;
	usart2.USART_Parity=USART_Parity_No;
	usart2.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&usart2);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	
}

void USART2_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		res=USART_ReceiveData(USART2);
		printf(" 接收数据：%d \r\n ",res);
		switch(res>>6)
		{
			case 1:
				printf(" 颜色识别模式\r\n ");	
				switch(res&0x07)
				{
					case 1:
						printf("RBG\r\n");
					break;
					case 2:
						printf("RGB\r\n");
					break;
					case 3:
						printf("BRG\r\n");
					break;
					case 4:
						printf("BGR\r\n");
					break;
					case 5:
						printf("GRB\r\n");
					break;
					case 6:
						printf("GBR\r\n");
					break;
				}
				break;
			case 2:
				printf(" 投放校准模式\r\n ");
			  switch((res>>4)&0x03)
				{
					case 0:
						printf(" 小车后退 %d mm\r\n ",(res&0x0f)*4);
					break;
					case 1:
						printf(" 小车右移 %d mm\r\n ",(res&0x0f)*4);
					break;
					case 2:
						printf(" 小车左移 %d mm\r\n ",(res&0x0f)*4);
					break;
					case 3:
						printf(" 小车前进 %d mm\r\n ",(res&0x0f)*4);
					break;
				}
				break;
			case 3:
				printf(" 小车到位：投放物料！！！\r\n ");
			break;
		}
	}
	
}


