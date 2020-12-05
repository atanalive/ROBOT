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
	u8 infrared_front;//����ѭ�������߷�����Ϣ(ǰ��)
	u8 infrared_right;//����ѭ�������߷�����Ϣ(�ұ�)
	u8 front_black_count;//(ǰ��)���߼���
	u8 right_black_count;//(�ұ�)���߼���
	u32 count_time;//У׼ʱ��洢
	float mpu6050_info;//�����Ǵ�����Ϣ
};



extern struct FEEDBACK feedback;


void update_robot_state(void);
void wheelvelocity_to_coordinateAxis_changeover(void);
void calibration_interrupt(enum ROBOT_STATE_TYPE LAST,enum CALIBRATION_TYPE TYPE);
void turnaround_interrupt(enum ROBOT_STATE_TYPE LAST);
void step_back_wall_calibration(void);
void black_calibration(void);
void pathover_calibration(void);

#endif


