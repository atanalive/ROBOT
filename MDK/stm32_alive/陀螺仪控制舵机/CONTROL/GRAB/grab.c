#include "grab.h"
#include "mg995.h"
#include "delay.h"

void grab_object(void)
{
	PWMUP=steering_calculate_angle(0);//1ms,45���½�
	delay_ms(1000);
	PWMUP=steering_calculate_angle(180);//1ms,45���½�
	delay_ms(1000);
	/*
  PWMUP=steering_calculate_angle(45);//1ms,45���½�
	delay_ms(1000);
	PWMGRAB=steering_calculate_angle(45);//1ms,45��ץȡ
	delay_ms(1000);*/
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

u16 steering_calculate_angle(float angle)//��װ����ĽǶ���ռ�ձ�
{
	return angle*500/45+500;
}


