#include "usart3.h"
#include "vision.h"
#include "fsm.h"

//接收树莓派数据(串口波特率我暂时设置为115200)
//PB10  PB11
void usart3_init(u32 bound)
{
	GPIO_InitTypeDef  gpio;
	USART_InitTypeDef usart3;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//USART3_TX   GPIOA.9
	gpio.GPIO_Pin=GPIO_Pin_10;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
	//USART1_RX	  GPIOA.10初始化
	gpio.GPIO_Pin=GPIO_Pin_11;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&gpio);
	
	//NVIC配置
	nvic.NVIC_IRQChannel=USART3_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=3;
	nvic.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&nvic);
	
	//usart初始化
	usart3.USART_BaudRate=bound;//串口波特率
	usart3.USART_WordLength=USART_WordLength_8b;//字节为8位数据格式
	usart3.USART_Parity=USART_Parity_No;//无奇偶校验位
	usart3.USART_StopBits=USART_StopBits_1;//一个停止位
	//停止位主要是通知接收设备，本帧传输完成，如果没有停止位，接收设备会一直处在接收状态，没办法发送下一帧数据
	usart3.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//无硬件数据流控制
	//对于硬件数据流控制一般用于半双工时的收发切换（RTS:外部请求发送标志位/CTS：本设备是否空闲能否接收标志位）防止未准备好就发送数据造成数据丢失。
	usart3.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//收发模式
	
	USART_Init(USART3,&usart3);//初始化串口
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//开启串口接收中断
	USART_Cmd(USART3,ENABLE);//使能串口
}

//(0x0d,0x0a是按回车发送的)
void USART3_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
		{
		 res =USART_ReceiveData(USART3);	//读取接收到的数据
		 if(ROBOT_VISION==VISION_START)
     {
			 VISION.receive=res;
		 }
    } 
}



