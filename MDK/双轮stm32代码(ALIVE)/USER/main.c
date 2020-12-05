#include "main.h"

float stand_angle=0;//校准角度
float old_angle=0;//前一次角度
float before_angle=0;//前两次角度
float left_angle=0;//左摆角度
float right_angle=0;//右摆角度
float balance_angle=0;//平衡角度
//float kp=0;//动态加速度
u8 left_flag=0;//标志位：用于判断是否采集够一组数据
u8 right_flag=0;
/*************************************************************************************************/
/***************************************ATANALIVE*************************************************/
/*************************************************************************************************/
extern PID velocity_pid[2];
PID slow_velocity_pid[2];
extern PID stand_pid;
/*unsigned char i;//计数变量
unsigned char Send_Count; //串口发送的数据个数
static float a=0;
*/

 int main(void)
 {		
		u8 i;
		float output[2]={0};
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		delay_init(); 	    	 //延时函数初始化	  
		uart_init(115200);	 //串口初始化为115200
		
		Init_all();
		//task_frequence_loop();
		pidinit(&stand_pid,10,0,0,2500,0,20);//直立环
		for(i=0;i<2;i++)
		{
			pidinit(&velocity_pid[i],10,3.8,0,2500,500,100);//速度环
			pidinit(&slow_velocity_pid[i],1,0.5,0,2000,1000,100);//减速环
		}
		
		//pidinit(&slow_velocity_pid[0],5,3,2,2000,500,20);//左轮pid
		//pidinit(&slow_velocity_pid[1],5,3,2,2000,500,20);//右轮pid
//		robot.target_wheel_v[0]=0;
//		robot.target_wheel_v[1]=0;
		while(1)
		{
			
			robot_get_anger();
			
			//直立环pid
			pid_position_calculate(&stand_pid,robot.anger,stand_angle);
			
			//自动调整平衡位置(alive)
			balance_auto_paragram_handle(2);
			

			//速度环pid
			for(i=0;i<2;i++)
			{
//				if(stand_pid.output<0)kp=-stand_pid.output;
//				else{kp=stand_pid.output;}
//				pidinit(&velocity_pid[i],kp,9,5,2500,500,100);//速度环
				pid_position_calculate(&velocity_pid[i],robot.wheel_v[i],stand_pid.output);
				pid_position_calculate(&slow_velocity_pid[i],robot.wheel_v[i],0);
			}
		
			//设置pwm
			for(i=0;i<2;i++)
			{
				//output[i]+=stand_pid.output;//直立环pid叠加
				
				output[i]+=velocity_pid[i].output;//速度环pid叠加

				output[i]+=slow_velocity_pid[i].output;//控速环叠加
				
				output[i]=output_limit(output[i],2500);
				//set_motor(i+3,output[i]);
				set_motor(i+3,velocity_pid[i].output);
			}
			delay_ms(5);
		}
		
   //上位机代码
 //while(1)
	//	{
	
	 //代码区
	 
		/*DataScope_Get_Channel_Data(encoder.v[0], 1 );
		DataScope_Get_Channel_Data(encoder.target_velocity[0], 2 );
		DataScope_Get_Channel_Data(encoder.v[1], 3 );
		DataScope_Get_Channel_Data(encoder.target_velocity[1], 4 );
		DataScope_Get_Channel_Data(encoder.v[2], 5 );
		DataScope_Get_Channel_Data(encoder.target_velocity[2], 6 );
*/
/*	Send_Count = DataScope_Data_Generate(10);
		for( i = 0 ; i < Send_Count; i++)
		{
		while((USART1->SR&0X40)==0);
		USART1->DR = DataScope_OutPut_Buffer[i];
		}
		delay_ms(50); //20HZ
		}
*/
}
//平衡位置自动调参
void balance_auto_paragram_handle(float set_anger)
{
	if(old_angle-before_angle<0&&robot.anger-old_angle>0)
	{
		left_angle=old_angle;
		left_flag=1;
	}
	else if(old_angle-before_angle>0&&robot.anger-old_angle<0)
	{
		right_angle=old_angle;
		right_flag=1;
	}
	if(left_flag&&right_flag)
	{
		left_flag=right_flag=0;
		balance_angle=(left_angle+right_angle)/2;
		if(balance_angle-stand_angle>set_anger)
		{
			if(balance_angle>stand_angle)
			{
				stand_angle-=0.5;
			}
			else if(balance_angle<stand_angle)
			{
				stand_angle+=0.5;
			}
		}
//		if(stand_angle>4)stand_angle-=1;
//		else if(stand_angle<-4)stand_angle+=1;
	}
	old_angle=robot.anger;//更新参数
	before_angle=old_angle;
}

//pid自动调参
void pid_auto_paragram_handle()
{
	u8 i;
	if(robot.anger<20|robot.anger>-20)
	{
		pidinit(&stand_pid,10,0,10,1800,0,20);//直立环
		for(i=0;i<2;i++)
		{
			pidinit(&velocity_pid[i],12,5,0,3000,500,20);//左右轮pid
		}
	}
	else if(robot.anger<40|robot.anger>-40)
	{
		pidinit(&stand_pid,20,0,20,1800,0,20);//直立环
		for(i=0;i<2;i++)
		{
			pidinit(&velocity_pid[i],24,10,0,3000,500,20);//左右轮pid
		}
	}
}

void Init_all()
{
	//encoder_init();
	pwminit(3600-1,0);//20kHz
	Motor_Init();
	//pid_all_init();
	MPU_all_init();
	//timinit(36000-1,20-1);//10ms
}
