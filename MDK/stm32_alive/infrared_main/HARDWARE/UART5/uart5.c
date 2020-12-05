#include "uart5.h"
#include "usart.h"

void uart5_init(u32 bound)
{
	//定义结构体
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef uart5;
	NVIC_InitTypeDef nvic;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	
	
	//GPIO初始化
	//UART4 TX PC10
	gpio.GPIO_Pin=GPIO_Pin_12;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	//UART4 RX PC11
	gpio.GPIO_Pin=GPIO_Pin_2;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&gpio);
	
	//UART4 NVIC 配置
	nvic.NVIC_IRQChannel=UART5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority=2;
	nvic.NVIC_IRQChannelSubPriority=2;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	
	//usart4 初始化
	uart5.USART_BaudRate=bound;
	uart5.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	uart5.USART_WordLength=USART_WordLength_8b;
	uart5.USART_StopBits=USART_StopBits_1;
	uart5.USART_Parity=USART_Parity_No;
	uart5.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5,&uart5);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART5,ENABLE);
	
	
}

void UART5_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==SET)
	{
		
		res=USART_ReceiveData(UART5);
		//printf("%d \r\n",res);
		USART_SendData(USART1,res);
		USART_ClearFlag(UART5, USART_IT_RXNE); //清除标志位；
	}
	
}


