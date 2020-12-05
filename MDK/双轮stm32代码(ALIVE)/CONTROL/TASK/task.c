#include "main.h"
#include "task.h"

struct TIME time={0};

void task_frequence_loop(void)
{
	//printf("我进入task_frequence_loop了\r\n");
	while(1)
  {
		if(1)//检查按钮转状态,只有当按钮波动了之后才能启动小车的核心运行程序
		{
			
			//printf("我进入if了\r\n");
			/*if(time.taskcount%1==0)//1ms
			{
				
			}
			if(time.taskcount%2==0)//2ms
			{
				
				//update_robot_state();
			}
			if(time.taskcount%5==0)//5ms执行一次
			{
				
			}*/
			
			if(time.taskcount%1==0)//10ms
			{
				//printf("do\r\n");
				//printf("encoder.v[0] is %f ;\r\n encoder.v[1] is %f ;\r\n encoder.v[2] is %f ;\r\n encoder.target_velocity[0] is %f ;\r\nencoder.target_velocity[1] is %f ;\r\nencoder.target_velocity[2] is %f ;\r\n",
					//	encoder.v[0],encoder.v[1],encoder.v[2],encoder.target_velocity[0],encoder.target_velocity[1],encoder.target_velocity[2]);
				//速度控制
				CONTROL_VELOCITY();
			}
			/*if(time.taskcount%20==0)//20ms
			{
				
			}
			if(time.taskcount%50==0)//50ms
			{
				
			}
			if(time.taskcount%100==0)//100ms
			{
				
			}
			if(time.taskcount%200==0)//200ms
			{
				
			}
			if(time.taskcount%500==0)//500ms
			{
				
			}
			if(time.taskcount%1000==0)//1000ms
			{
				
			}*/
			if(time.taskcount%500==0)//5s
			{
				
			}
			//printf("do1\r\n");
			if(time.taskcount%1000==0)//10s
			{
				
			}
			//printf("do2\r\n");
			if(time.taskcount%1500==0)//15s
			{
				
			}
			//printf("do3\r\n");
			if(time.taskcount%2000==0)//20s
			{
				
			}
			//printf("do4\r\n");
			if(time.taskcount%2500==0)//20s
			{
				
			}
			//printf("do5\r\n");
		}
		//printf("time.taskcount is %d\r\n",time.taskcount);
	}
}


//速度控制
void CONTROL_VELOCITY(void)
{
	/*更新车子自身数据*/
	//calculate_encoder_velocity();
	robot_get_anger();
	/*车子速度pid控制*/
	motor_velocity_control();
	
}


