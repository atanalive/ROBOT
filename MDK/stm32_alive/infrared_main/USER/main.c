#include "stm32f10x.h"
#include "uart4.h"
#include "usart.h"
#include "delay.h"
#include "uart5.h"
/************************************************
 ALIENTEK ս��STM32F103������ʵ��0
 ����ģ��
 ע�⣬�����ֲ��е��½������½�ʹ�õ�main�ļ� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


int main(void)
{		
	// u8 i;
	 delay_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 00|00 ����λ
	 uart_init(115200);
	 uart4_init(115200);
	 while(1)
	{
//		USART_SendData(USART2,0x80);
		//printf("ok \r\n");
		/*
		

		USART_SendData(USART2,0x81);
		USART_SendData(USART2,0x82);
		USART_SendData(USART2,0x83);
		USART_SendData(USART2,0x84);
		USART_SendData(USART2,0x85);
		USART_SendData(USART2,0x86);*/
	}
	 
	//while(1);
}








