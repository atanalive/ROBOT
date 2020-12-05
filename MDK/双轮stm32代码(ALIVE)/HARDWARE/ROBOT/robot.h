#ifndef ROBOT_H
#define	ROBOT_H
#include "sys.h"
struct ROBOT
{	
	float wheel_v[2];  //三个轮子速度
	float target_wheel_v[2];//目标速度
	float anger;//倾斜角
	float x_v;//x轴速度
	float y_v;//y轴速度
	
};
extern struct ROBOT robot;

void robot_get_anger(void);
#endif

