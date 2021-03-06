#include "fsm.h"
#include "main.h"

#define send_data_to_vision_times 3  //发送3次数据给树莓派
/***************定义区****************/
enum ROBOT_STATE_TYPE ROBOT_STATE;//当前状态
struct ROBOT_NOW_TYPE ROBOT_NOW={0};//机器人自身实时状态
struct state_control STATE_COUNT={1,1};//计数控制状态
enum CALIBRATION_TYPE ROBOT_CALIBRATION;//校准状态
enum ROBOT_VISION_TYPE ROBOT_VISION;//视觉开关
enum ROBOT_GRAB_TYPE ROBOT_GRAB;//抓取状态
enum ROBOT_PUT_TYPE ROBOT_PUT;//投放
enum ROBOT_TRACKING_TYPE ROBOT_TRACKING;//移动

//有限状态机初始化
void fsm_init(void)
{
	ROBOT_STATE=CALIBRATION;
	ROBOT_GRAB=GRAB_STOP;
	ROBOT_TRACKING=TRACKING_STOP;
	ROBOT_PUT=PUT_STOP;
	ROBOT_VISION=VISION_STOP;
	ROBOT_CALIBRATION=CALIBRATION_STOP;
}


//状态机
void FSM_WORK(void)//起到切换状态的作用
{
	switch(ROBOT_STATE)
  {
		//待机模式（当回到起始区时进入待机模式）
		case STANDBY:
		{
			ROBOT_GRAB=GRAB_STOP;
      ROBOT_TRACKING=TRACKING_STOP;
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_VISION=VISION_STOP;
			ROBOT_CALIBRATION=CALIBRATION_STOP;
		}
		break;
		//校准模式(自编中断触发)
		case CALIBRATION:
		{

			ROBOT_GRAB=GRAB_STOP;
			ROBOT_TRACKING=TRACKING_STOP;
			ROBOT_PUT=PUT_STOP;
			ROBOT_VISION=VISION_STOP;
			ROBOT_CALIBRATION=CALIBRATION_START;
			
			if(ROBOT_CALIBRATION==CALIBRATION_SUCCEED)
			{
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				VISION_SEND_CONTROL=VISION_ASK_START;//发送数据给树莓派表示开始
				ROBOT_CALIBRATION=CALIBRATION_STOP;
				ROBOT_STATE=START_TO_MIDDLE;
			}
			
		}
		break;
		//开始到中间
		case START_TO_MIDDLE:
		{
		  ROBOT_GRAB=GRAB_STOP;
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_VISION=VISION_STOP;
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEED)
			{		
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				VISION_SEND_CONTROL=VISION_ASK_JUDGEMENT;//发送数据给树莓派开始识别
				VISION.send_count=send_data_to_vision_times;//发送数据次数设置
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=VISION_JUDGEMENT;
			}
		}
		break;
		case VISION_JUDGEMENT:
		{
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_TRACKING=TRACKING_STOP;
			
			ROBOT_VISION=VISION_START_RECEIVE;//开始接收视觉数据
			if(ROBOT_VISION==VISION_SUCCEED)
			{
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				ROBOT_VISION=VISION_STOP;
				ROBOT_STATE=MIDDLE_TO_GRAB;
			}
		}
		break;
		case MIDDLE_TO_GRAB://中间到物料台
		{
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEED)
      {
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=GRAB;
			}
		}
		break;
		case GRAB://抓取
		{
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_TRACKING=TRACKING_STOP;
			
			ROBOT_GRAB=GRAB_START;
			if(ROBOT_GRAB==GRAB_SUCCEED)
      {
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				ROBOT_GRAB=GRAB_STOP;
			}
		}
		break;
		case GRAB_TO_PUT:
		{
			ROBOT_PUT=PUT_STOP;
			ROBOT_GRAB=GRAB_STOP;
			ROBOT_VISION=VISION_STOP;
			ROBOT_CALIBRATION=CALIBRATION_STOP;
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEED)
			{
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=PUT;
			}
		}
		break;
		case PUT://放置
		{
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_TRACKING=TRACKING_STOP;
			
			ROBOT_PUT=PUT_START;			
			if(ROBOT_PUT==PUT_SUCCEED)
      {
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				ROBOT_PUT=PUT_STOP;
				STATE_COUNT.TURNAROUND_TO_PUT_count++;
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==4)//抓取投放工作完成
				{
					ROBOT_STATE=RETURN_TO_START;
					
				}
			}
		}
		break;
		case RETURN_TO_GRAB://转身后回到物料台
		{
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_PUT=PUT_STOP;	
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEED)
      {
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				ROBOT_TRACKING=TRACKING_STOP;
				STATE_COUNT.TURNAROUND_TO_TABLE_count++;
				ROBOT_STATE=GRAB;
			}
		}
		break;
		case RETURN_TO_START://结束回到起始区
    {
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_PUT=PUT_STOP;	
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEED)
      {
				infrared_init();//循迹数据初始化，便于下一个路段再次使用
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=STANDBY;
			}
		}
		break;
	}
}

//循迹数据初始化，便于下一个路段再次使用
void infrared_init(void)
{
	time.taskcount_store=time.taskcount;//每一个路段开始前重新计时
	feedback.behind_black_count=0;
	feedback.right_black_count=0;
}


