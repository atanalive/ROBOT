#include "main.h"
#include "math.h"
#include "moving.h"
#include "usart.h"
#include "encoder.h"

float theta;
static u8 a=1;

//世界坐标速度转换成轮子速度
//单轮为前，双纶为后
void coordinates_to_wheelvelocity_changeover(void)
{
	printf("目标速度");
	if(a==1){ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw;a++;}//得到初始角度
  theta=pi*(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw)/180.0f;//转为弧度制，pi定义在main.h文件中
	
	encoder.target_velocity[0]=-cos(theta)*ROBOT_NOW.target_Vx-sin(theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[1]=cos(pi/3+theta)*ROBOT_NOW.target_Vx+sin(pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[2]=cos(-pi/3+theta)*ROBOT_NOW.target_Vx+sin(-pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
}

