#include "main.h"


int main(void)
{	
	float angle=90;
	float chage=5;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	
	pca_init(50);
	
	while(1)
	{
		set_steer_angle(0,angle);
		angle+=chage;
		if(angle==175)chage=-5;
		else if(angle==5)chage=5;
		delay_ms(200);
	}
	
}
