#include "uart4.h"
#include "usart.h"

void uart4_init(u32 bound)
{
	//定义结构体
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart4;
	NVIC_InitTypeDef nvic;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	
	//GPIO初始化
	//UART4 TX PC10
	gpio.GPIO_Pin=GPIO_Pin_10;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	//UART4 RX PC11
	gpio.GPIO_Pin=GPIO_Pin_11;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&gpio);
	
	//UART4 NVIC 配置
	nvic.NVIC_IRQChannel=UART4_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=2;
	nvic.NVIC_IRQChannelSubPriority=2;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	
	//usart4 初始化
	uart4.USART_BaudRate=bound;
	uart4.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	uart4.USART_WordLength=USART_WordLength_8b;
	uart4.USART_StopBits=USART_StopBits_1;
	uart4.USART_Parity=USART_Parity_No;
	uart4.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4,&uart4);
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART4,ENABLE);
	
	
}

void UART4_IRQHandler(void)
{
	u8 res;
	//printf("now\r\n");
	if(USART_GetITStatus(UART4,USART_IT_RXNE) == SET)
	{
		//printf("我进入中断了\r\n");
		res=USART_ReceiveData(UART4);
		//printf("%d \r\n",res);
		USART_SendData(USART1,res);
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);	
	}
}


