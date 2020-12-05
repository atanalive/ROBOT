#ifndef __FEEDBACK_H
#define __FEEDBACK_H
#include "sys.h"
#include "fsm.h"
#include "encoder.h"
#include "mpu6050.h"
#include "math.h"
#include "task.h"
#include "main.h"

//һ���������ṹ��
struct FEEDBACK
{
	u8 infrared_value;//�洢����ѭ��ģ�鷵�ص�ֵ
//	u8 infrared_front;//����ѭ�������߷�����Ϣ(ǰ��)
//	u8 infrared_right;//����ѭ�������߷�����Ϣ(�ұ�)
	u8 behind_black_count;//(����)���߼���
	u8 right_black_count;//(�ұ�)���߼���
	float mpu6050_info;//�����Ǵ�����Ϣ
};



extern struct FEEDBACK feedback;


void update_robot_state(void);
void wheelvelocity_to_coordinateAxis_changeover(void);


#endif


