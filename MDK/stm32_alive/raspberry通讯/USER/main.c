#include "stm32f10x.h"
#include "raspberry.h"
#include "usart.h"

/************************************************
 ALIENTEK 战舰STM32F103开发板实验0
 工程模板
 注意，这是手册中的新建工程章节使用的main文件 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


int main(void)
{		
	// u8 i;
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 00|00 各两位
	 uart_init(115200);
	 usart2_init(115200);
	 while(1)
	{
		/*
		USART_SendData(USART2,0x80);
		USART_SendData(USART2,0x81);
		USART_SendData(USART2,0x82);
		USART_SendData(USART2,0x83);
		USART_SendData(USART2,0x84);
		USART_SendData(USART2,0x85);
		USART_SendData(USART2,0x86);*/
	}
	 
	//while(1);
}








