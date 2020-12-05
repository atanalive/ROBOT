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

//一个反馈量结构体
struct FEEDBACK
{
	u8 trace_infrared;//储存循迹红外线反馈信息(这个反回的信息还不知道，待定)
	u32 count_time;//校准时间存储
	u8 control_calibration;//校准时间存储控制
	u8 black_calibration;//黑线校准开关
	u8 wall_calibration;//顶墙校准开关
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


