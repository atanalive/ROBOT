#include "uart5.h"
#include "feedback.h"

//用于循迹红外传感器
//PC12(TX)  PD2(RX)
void uart5_init(u32 bound)
{
	GPIO_InitTypeDef  gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	
	//USART2_TX 输出  GPIOC12
	gpio.GPIO_Pin=GPIO_Pin_12;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	//USART2_RX	输入  GPIOD2初始化
	gpio.GPIO_Pin=GPIO_Pin_2;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&gpio);
	
	//NVIC配置
	nvic.NVIC_IRQChannel=UART5_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=2;
	nvic.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&nvic);
	
	//usart初始化
	usart.USART_BaudRate=bound;//串口波特率
	usart.USART_WordLength=USART_WordLength_8b;//字节为8位数据格式
	usart.USART_Parity=USART_Parity_No;//无奇偶校验位
	usart.USART_StopBits=USART_StopBits_1;//一个停止位
	//停止位主要是通知接收设备，本帧传输完成，如果没有停止位，接收设备会一直处在接收状态，没办法发送下一帧数据
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
	//对于硬件数据流控制一般用于半双工时的收发切换（RTS:外部请求发送标志位/CTS：本设备是否空闲能否接收标志位）防止未准备好就发送数据造成数据丢失。
	//usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//收发模式
	usart.USART_Mode=USART_Mode_Rx;//接收模式
	
	USART_Init(UART5,&usart);//初始化串口
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);//开启串口接收中断
	USART_Cmd(UART5,ENABLE);//使能串口
}

//(0x0d,0x0a是按回车发送的)
void UART5_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断
		{
		 res =USART_ReceiveData(UART5);	//读取接收到的数据
		 feedback.infrared_right=res;
    } 
}
