#ifndef __FEEDBACK_H
#define __FEEDBACK_H
#include "sys.h"
#include "vl53l0x_gen.h"
#include "fsm.h"
#include "encoder.h"
#include "mpu6050.h"
#include "math.h"
#include "main.h"
#include "task.h"
#include "main.h"

//һ���������ṹ��
struct FEEDBACK
{
	u8 trace_infrared;//����ѭ�������߷�����Ϣ(������ص���Ϣ����֪��������)
	u32 count_time;//У׼ʱ��洢
	u8 control_calibration;//У׼ʱ��洢����
	u8 black_calibration;//����У׼����
	u8 wall_calibration;//��ǽУ׼����
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


