#ifndef ROBOT_H
#define	ROBOT_H
#include "sys.h"
struct ROBOT
{	
	float wheel_v[2];  //���������ٶ�
	float target_wheel_v[2];//Ŀ���ٶ�
	float anger;//��б��
	float x_v;//x���ٶ�
	float y_v;//y���ٶ�
	
};
extern struct ROBOT robot;

void robot_get_anger(void);
#endif

