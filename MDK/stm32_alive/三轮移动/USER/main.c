#include "main.h"

/************************************************
 ALIENTEK战舰STM32开发板实验9
 PWM输出实验  
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
/*unsigned char i;//计数变量
unsigned char Send_Count; //串口发送的数据个数
static float a=0;
*/
struct ROBOT ROBOT_NOW={0};

 int main(void)
 {		
	 
	 
	 
//	Stm32_Clock_Init(9);      //系统时钟设置
// 	u16 led0pwmval=0;
//	u8 dir=1;	
	delay_init(); 	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
	encoder_init();
	timinit(36000-1,20-1);//10ms
	pwminit(3600-1,0);//20kHz
	Motor_Init();
	control_encoder_init();
	
  ROBOT_NOW.target_Vx=10;
	ROBOT_NOW.target_Vy=0;
	ROBOT_NOW.target_w=0;
	coordinates_to_wheelvelocity_changeover();
	

   //上位机代码
 //while(1)
	//	{
			
		//coordinates_to_wheelvelocity_changeover();
		//pid_position_calculate(&test,encoder.v[0],encoder.target_velocity[0]);
		//printf("encoder.v[0] is %f ;\r\n encoder.v[1] is %f ;\r\n encoder.v[2] is %f ;\r\n encoder.target_velocity[0] is %f ;\r\nencoder.target_velocity[1] is %f ;\r\nencoder.target_velocity[2] is %f ;\r\n",
						//encoder.v[0],encoder.v[1],encoder.v[2],encoder.target_velocity[0],encoder.target_velocity[1],encoder.target_velocity[2]);
		//set_motor(3,&test);
			
		  task_frequence_loop();
	//	}
	/*		
		a+=0.1;
		if(a>3.14)  a=-3.14; */
	/*	if(time.taskcount>=500)
		{
			ROBOT_NOW.target_Vx=-10;
			ROBOT_NOW.target_Vy=0;
		}*/
		/*DataScope_Get_Channel_Data(encoder.v[0], 1 );
		DataScope_Get_Channel_Data(encoder.target_velocity[0], 2 );
		DataScope_Get_Channel_Data(encoder.v[1], 3 );
		DataScope_Get_Channel_Data(encoder.target_velocity[1], 4 );
		DataScope_Get_Channel_Data(encoder.v[2], 5 );
		DataScope_Get_Channel_Data(encoder.target_velocity[2], 6 );
*/
/*		DataScope_Get_Channel_Data( 500*cos(a), 3 );
		DataScope_Get_Channel_Data( 100*a , 4 );
		DataScope_Get_Channel_Data(0, 5 );
		DataScope_Get_Channel_Data(0 , 6 );
		DataScope_Get_Channel_Data(0, 7 );
		DataScope_Get_Channel_Data( 0, 8 );
		DataScope_Get_Channel_Data(0, 9 );
		DataScope_Get_Channel_Data( 0 , 10);*/
/*		Send_Count = DataScope_Data_Generate(10);
		for( i = 0 ; i < Send_Count; i++)
		{
		while((USART1->SR&0X40)==0);
		USART1->DR = DataScope_OutPut_Buffer[i];
		}
		delay_ms(50); //20HZ
		}
*/
}
