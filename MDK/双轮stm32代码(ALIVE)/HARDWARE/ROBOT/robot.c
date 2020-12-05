#include "robot.h"
#include "mpu6050.h"
struct ROBOT robot={{0},{0},0,0,0};
float pitch,roll,yaw;

void robot_get_anger(void)
{
	//pitch yaw roll 分别为x,y,z轴坐标
	mpu_dmp_get_data(&pitch,&roll,&yaw);
//	if(roll>20&&roll>-20)roll*=4;
//	else if(roll>10&&roll>-10)roll*=2;
	robot.anger=roll;//
	
}

