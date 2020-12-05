#include "main.h"
/***********ATANALIVE**************/


int main(void)
{	
	u8 send_data=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();
	Adc_Init();		  		//ADC��ʼ��
	
	
	while(1)
	{
		
		#if RIGHT_INFRARED
		send_data=infrared_right_value_handle();//�ұߴ�����
		#endif
		#if FRONT_INFRARED
		send_data=infrared_front_value_handle();//ǰ�洫����
		#endif
		#if RIGHT_INFRARED||FRONT_INFRARED
		if(send_data)//������صĲ���0
		{
			#if INFRARED_DEBUG==0
			USART_SendData(USART1,send_data);
			#endif
		}
		#endif
	}
	
}


