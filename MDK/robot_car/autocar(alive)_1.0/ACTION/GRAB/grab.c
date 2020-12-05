#include "grab.h"
#include "mg995.h"
#include "delay.h"
#include "fsm.h"
#include "vision.h"

void grab_object(void)
{
  PWMUP=steering_calculate_angle(45);//1ms,45���½�
	delay_ms(1000);
	PWMGRAB=steering_calculate_angle(45);//1ms,45��ץȡ
	delay_ms(1000);
}


void put_object(void)
{
	PWMUP=steering_calculate_angle(90);//�½�90��
	delay_ms(1000);
	PWMGRAB=steering_calculate_angle(0);//צ�ӷ���
	delay_ms(500);
	PWMUP=steering_calculate_angle(0);//����
	delay_ms(1000);
}

u8 steering_calculate_angle(u8 angle)//��װ����ĽǶ���ռ�ձ�
{
	return angle/90+5;
}

//Ͷ��orץȡ
void grab_or_put(void)
{
	if(ROBOT_GRAB==GRAB_START)
  {
		grab_object();
		delay_ms(100);
		if(VISION.grab==0x07){ROBOT_GRAB=GRAB_SUCCEEDED;VISION.grab=0;}
		return;
	}
	else if(ROBOT_PUT==PUT_START)
  {
		put_object();
    delay_ms(100);
    if(VISION.put==1){ROBOT_PUT=PUT_SUCCEEDED;VISION.grab=0;}		
	}
}


