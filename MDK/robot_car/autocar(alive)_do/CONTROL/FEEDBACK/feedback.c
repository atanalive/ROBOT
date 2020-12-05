#include "feedback.h"
#include "path.h"
#include "pid.h"
#include "math.h"
#include "main.h"
/***************反馈更新小车的信息********************/

struct FEEDBACK feedback={0};

//反馈得到机器人的世界轴实际速度
//轮子速度转换成世界坐标速度(求逆,或者写出v与小车坐标轴的矩阵再通过旋转变换得到)(前者耗cpu,后者人工算出结果矩阵即可)
void wheelvelocity_to_coordinateAxis_changeover(void)//coordinate Axis:坐标轴
{
	float thetainit=ROBOT_NOW.initial_yaw*pi/180.0f;//三角函数用的是弧度制
	float theta=(ROBOT_NOW.yaw*pi/180.0f)-thetainit;
	ROBOT_NOW.Vx=(-cos(theta))*encoder.v[0]+(1.0f/2*cos(theta)+sqrt(3)/2.0f*sin(theta))*encoder.v[1]+(1.0f/2*cos(theta)-sqrt(3)/2.0f*sin(theta))*encoder.v[2];
	ROBOT_NOW.Vy=sin(theta)*encoder.v[0]+(-1.0f/2*sin(theta)+sqrt(3)/2.0f*cos(theta))*encoder.v[1]+(-1.0f/2*sin(theta)-sqrt(3)/2.0f*cos(theta))*encoder.v[2];
	//ROBOT_NOW.w=L*(encoder.v[0]+encoder.v[1]+encoder.v[2]);
}


//陀螺仪获得角度和角速度
void update_robot_state(void)
{
  mpu_dmp_get_yaw_data(&ROBOT_NOW.yaw);
	ROBOT_NOW.w=get_yaw_angular_velocity();
	
}


