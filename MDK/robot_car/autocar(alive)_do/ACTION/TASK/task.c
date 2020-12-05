#include "main.h"

struct TIME time={0};
#define velocity 30

void task_frequence_loop(void)
{
	while(1)
  {
		if(1)//检查按钮转状态,只有当按钮波动了之后才能启动小车的核心运行程序
		{
			if(time.taskcount%1==0)//1ms
			{
				//FSM_WORK();
			}
			else if(time.taskcount%2==0)//2ms
			{
				motor_velocity_control();
				//update_robot_state();
			}
			else if(time.taskcount%5==0)//5ms执行一次
			{
				CALMULATE();
			}
			else if(time.taskcount%10==0)//10ms
			{
				//ACTION();
			}
			else if(time.taskcount%20==0)//20ms
			{
				
			}
			else if(time.taskcount%50==0)//50ms
			{
				
			}
			else if(time.taskcount%100==0)//100ms
			{
				
			}
			else if(time.taskcount%200==0)//200ms
			{
				
			}
			else if(time.taskcount%500==0)//500ms
			{
				
			}
			else if(time.taskcount%2000==0)//2000ms
			{
				static int a=0;
				if(a%4==0)
				{
					ROBOT_NOW.target_Vy=0;
					ROBOT_NOW.target_Vx=10;
				}
				else if(a%4==1)
				{
					ROBOT_NOW.target_Vy=0;
					ROBOT_NOW.target_Vx=-10;
				}
				else if(a%4==1)
				{
					ROBOT_NOW.target_Vy=10;
					ROBOT_NOW.target_Vx=0;
				}
				else if(a%4==1)
				{
					ROBOT_NOW.target_Vy=-10;
					ROBOT_NOW.target_Vx=0;
				}
				a++;
			}
			
		}
		
	}
	
}

void ACTION(void)
{
	grab_or_put();
	choice_path();
}

void CALMULATE(void)
{
	coordinates_to_wheelvelocity_changeover();
  //analytic_data_in_vision();
	//wheelvelocity_to_coordinateAxis_changeover();
}


