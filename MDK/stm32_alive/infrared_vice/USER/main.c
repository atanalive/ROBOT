#include "main.h"
/***********ATANALIVE**************/


int main(void)
{	
	u8 send_data=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();
	Adc_Init();		  		//ADC初始化
	
	
	while(1)
	{
		
		#if RIGHT_INFRARED
		send_data=infrared_right_value_handle();//右边传感器
		#endif
		#if FRONT_INFRARED
		send_data=infrared_front_value_handle();//前面传感器
		#endif
		#if RIGHT_INFRARED||FRONT_INFRARED
		if(send_data)//如果返回的不是0
		{
			#if INFRARED_DEBUG==0
			USART_SendData(USART1,send_data);
			#endif
		}
		#endif
	}
	
}


