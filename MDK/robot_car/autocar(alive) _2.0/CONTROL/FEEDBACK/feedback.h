#ifndef __FEEDBACK_H
#define __FEEDBACK_H
#include "sys.h"
#include "fsm.h"
#include "encoder.h"
#include "mpu6050.h"
#include "math.h"
#include "task.h"
#include "main.h"

//一个反馈量结构体
struct FEEDBACK
{
	u8 infrared_front;//储存循迹红外线反馈信息(前面)
	u8 infrared_right;//储存循迹红外线反馈信息(右边)
	u8 front_black_count;//(前面)黑线计数
	u8 right_black_count;//(右边)黑线计数
	u32 count_time;//校准时间存储
	float mpu6050_info;//陀螺仪串口信息
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


