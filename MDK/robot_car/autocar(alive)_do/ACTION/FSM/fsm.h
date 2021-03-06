#ifndef __FSM_H
#define __FSM_H
#include "sys.h"


//注意，头文件只能放声明，不能放定义，放定义的话如果多个文件引用会出现重复定义的错误
//当然，类型变量是可以的
//如果只给一个c用，h可以定义变量
/*****************机器人主状态*******************/
enum ROBOT_STATE_TYPE
{
	STANDBY,
	CALIBRATION,  //校准
	START_TO_MIDDLE,
	MIDDLE_TO_GRAB,
	GRAB,
	VISION_JUDGEMENT,
	GRAB_TO_PUT,
	PUT,
	RETURN_TO_GRAB,
	RETURN_TO_START,	
};

extern enum ROBOT_STATE_TYPE ROBOT_STATE;//当前状态
/********************机器人自身实时状态**************************/
#define L  58/2                //轮子半径L(mm)
#define length    300          //长度
#define width     210          //宽度
#define infrared  10           //红外传感器
#define car_length  100        //轮子与中心的距离

struct ROBOT_NOW_TYPE
{
	float X;//(X,Y由激光测距，循迹红外获得)
	float Y;
	float yaw;//航向角，角度制(由陀螺仪获得)
	float initial_yaw;//初始航向角(角度制)
	float Vx;//(编码器，矩阵计算)
	float Vy;
	float w;//yaw角速度，弧度制(陀螺仪获得)
	float target_Vx;//(速度规划获得)(世界坐标轴下的目标速度)
	float target_Vy;
	float target_w;
	u8 grab_order;//抓取次序(通过树莓派获得的排列次序和开关获得的抽签次序计算得到)
	u8 put_order;//摆放次序
};

extern struct ROBOT_NOW_TYPE ROBOT_NOW;//机器人自身实时状态

//抓取和摆放次序规定:(从左到右分别为abc)
/*
1:abc 2:acb 3:bac 4:bca 5:cab 6:cba
*/
/*****************计数控制状态******************/
struct state_control
{
  u8 TURNAROUND_TO_PUT_count;//TURNAROUND_TO_PUT状态执行计数
  u8 TURNAROUND_TO_TABLE_count;//TURNAROUND_TO_TABLE状态执行计数

};

extern struct state_control STATE_COUNT;//计数控制状态
/*****************机器人子状态********************/
//校准状态(校准的本质就是重新确定参照量)
enum CALIBRATION_TYPE
{
	CALIBRATION_STOP,
	CALIBRATION_START,
	CALIBRATION_SUCCEED,
};

extern enum CALIBRATION_TYPE ROBOT_CALIBRATION;//校准状态
//视觉开关(仅是开关接收树莓派信息的函数)
enum ROBOT_VISION_TYPE
{	
	VISION_STOP,
	VISION_START_RECEIVE,
	VISION_SUCCEED,
};

extern enum ROBOT_VISION_TYPE ROBOT_VISION;//视觉开关
//抓取状态
enum ROBOT_GRAB_TYPE
{
	GRAB_STOP,
	GRAB_START,
	GRAB_SUCCEED,
};

extern enum ROBOT_GRAB_TYPE ROBOT_GRAB;//抓取状态
//投放
enum ROBOT_PUT_TYPE
{
	PUT_STOP,
	PUT_START,
	PUT_SUCCEED,
};

extern enum ROBOT_PUT_TYPE ROBOT_PUT;//投放
//移动
enum ROBOT_TRACKING_TYPE
{
	TRACKING_STOP,
	TRACKING_START,
	TRACKING_SUCCEED,
};

extern enum ROBOT_TRACKING_TYPE ROBOT_TRACKING;//移动


void FSM_WORK(void);
void fsm_init(void);
void infrared_init(void);

#endif


