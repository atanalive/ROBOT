#include "main.h"


int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组
	timinit(3600-1,20-1);//设置定时器为1ms
	encoder_init();
	Motor_Init();
	control_encoder_init();
	ROBOT_NOW.target_Vy=0;
	ROBOT_NOW.target_Vx=10;
	
	//init_all();
	task_frequence_loop();//频率执行，伪多线程
	
}



void init_all(void)
{
	mg995_init(200-1,7200-1);//计一次数为0.1ms
	LED_Init();
	dial_switch_init();
	usart3_init(115200);
	usart2_init(115200);
	fsm_init();
}


