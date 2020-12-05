#include "feedback.h"
#include "path.h"
#include "pid.h"
#include "math.h"
#include "main.h"
/***************��������С������Ϣ********************/

struct FEEDBACK feedback={0};

//�����õ������˵�������ʵ���ٶ�
//�����ٶ�ת�������������ٶ�(����,����д��v��С��������ľ�����ͨ����ת�任�õ�)(ǰ�ߺ�cpu,�����˹����������󼴿�)
void wheelvelocity_to_coordinateAxis_changeover(void)//coordinate Axis:������
{
	float thetainit=ROBOT_NOW.initial_yaw*pi/180.0f;//���Ǻ����õ��ǻ�����
	float theta=(ROBOT_NOW.yaw*pi/180.0f)-thetainit;
	ROBOT_NOW.Vx=(-cos(theta))*encoder.v[0]+(1.0f/2*cos(theta)+sqrt(3)/2.0f*sin(theta))*encoder.v[1]+(1.0f/2*cos(theta)-sqrt(3)/2.0f*sin(theta))*encoder.v[2];
	ROBOT_NOW.Vy=sin(theta)*encoder.v[0]+(-1.0f/2*sin(theta)+sqrt(3)/2.0f*cos(theta))*encoder.v[1]+(-1.0f/2*sin(theta)-sqrt(3)/2.0f*cos(theta))*encoder.v[2];
	//ROBOT_NOW.w=L*(encoder.v[0]+encoder.v[1]+encoder.v[2]);
}


//�����ǻ�ýǶȺͽ��ٶ�
void update_robot_state(void)
{
  mpu_dmp_get_yaw_data(&ROBOT_NOW.yaw);
	ROBOT_NOW.w=get_yaw_angular_velocity();
	
}


