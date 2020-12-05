#include "main.h"
#include "math.h"
#include "moving.h"
#include "usart.h"
#include "encoder.h"

float theta;
static u8 a=1;

//���������ٶ�ת���������ٶ�
//����Ϊǰ��˫��Ϊ��
void coordinates_to_wheelvelocity_changeover(void)
{
	printf("Ŀ���ٶ�");
	if(a==1){ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw;a++;}//�õ���ʼ�Ƕ�
  theta=pi*(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw)/180.0f;//תΪ�����ƣ�pi������main.h�ļ���
	
	encoder.target_velocity[0]=-cos(theta)*ROBOT_NOW.target_Vx-sin(theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[1]=cos(pi/3+theta)*ROBOT_NOW.target_Vx+sin(pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[2]=cos(-pi/3+theta)*ROBOT_NOW.target_Vx+sin(-pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
}

