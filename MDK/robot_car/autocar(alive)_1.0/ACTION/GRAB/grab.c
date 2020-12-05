#include "grab.h"
#include "mg995.h"
#include "delay.h"
#include "fsm.h"
#include "vision.h"

void grab_object(void)
{
  PWMUP=steering_calculate_angle(45);//1ms,45°下降
	delay_ms(1000);
	PWMGRAB=steering_calculate_angle(45);//1ms,45°抓取
	delay_ms(1000);
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

u8 steering_calculate_angle(u8 angle)//封装舵机的角度与占空比
{
	return angle/90+5;
}

//投放or抓取
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


