#include "main.h"


int main(void)
{
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�����
	timinit(3600-1,20-1);//���ö�ʱ��Ϊ1ms
	encoder_init();
	Motor_Init();
	control_encoder_init();
	ROBOT_NOW.target_Vy=0;
	ROBOT_NOW.target_Vx=10;
	
	//init_all();
	task_frequence_loop();//Ƶ��ִ�У�α���߳�
	
}



void init_all(void)
{
	mg995_init(200-1,7200-1);//��һ����Ϊ0.1ms
	LED_Init();
	dial_switch_init();
	usart3_init(115200);
	usart2_init(115200);
	fsm_init();
}


