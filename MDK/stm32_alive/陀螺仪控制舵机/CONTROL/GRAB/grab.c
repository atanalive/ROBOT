#include "grab.h"
#include "mg995.h"
#include "delay.h"

void grab_object(void)
{
	PWMUP=steering_calculate_angle(0);//1ms,45°下降
	delay_ms(1000);
	PWMUP=steering_calculate_angle(180);//1ms,45°下降
	delay_ms(1000);
	/*
  PWMUP=steering_calculate_angle(45);//1ms,45°下降
	delay_ms(1000);
	PWMGRAB=steering_calculate_angle(45);//1ms,45°抓取
	delay_ms(1000);*/
}


void put_object(void)
{
	PWMUP=steering_calculate_angle(90);//下降90度
	delay_ms(1000);
	PWMGRAB=steering_calculate_angle(0);//爪子放手
	delay_ms(500);
	PWMUP=steering_calculate_angle(0);//上升
	delay_ms(1000);
}

u16 steering_calculate_angle(float angle)//封装舵机的角度与占空比
{
	return angle*500/45+500;
}


