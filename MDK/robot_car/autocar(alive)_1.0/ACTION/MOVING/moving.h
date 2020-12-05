#ifndef __MOVING_H
#define __MOVING_H
#include "sys.h"



struct MOVING_CONTROL
{
	float V_max;//�ٶ����ֵ����
	float V_low;//������ʻ�ٶ�
	u8 V_low_mode;//������ʻ����
	float s_slow_down;//���پ���
	float s_speed_up;//���پ���
	float a1;//���ٵļ��ٶ�
	float a2;//���ٵļ��ٶ�
};

//��������
extern struct MOVING_CONTROL moving;

float float_abs(float value);
void coordinates_to_wheelvelocity_changeover(void);
float moving_line_velocity_planning_trapezoid(float b,float s);
void turn_around(void);
void go_straight_or_turn_around(void);
void moving_line_track(void);
void whirling_180_angle(void);


#endif



