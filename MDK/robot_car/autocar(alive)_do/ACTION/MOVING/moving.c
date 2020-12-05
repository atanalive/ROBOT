#include "main.h"
#include "math.h"
#include "moving.h"
#include "path.h"
#include "fsm.h"
#include "usart.h"
#include "encoder.h"
#include "feedback.h"

float theta;
static u8 a=1;

//世界坐标速度转换成轮子速度
void coordinates_to_wheelvelocity_changeover(void)
{
	if(a==1){ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw;a++;}//得到初始角度
  theta=pi*(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw)/180.0f;//转为弧度制，pi定义在main.h文件中
	
	encoder.target_velocity[0]=sin(theta)*ROBOT_NOW.target_Vx-cos(theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[1]=sin(pi/3-theta)*ROBOT_NOW.target_Vx+cos(pi/3-theta)+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[2]=(-sin(pi/3+theta))*ROBOT_NOW.target_Vx+cos(pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
}

//循迹路径规划
void moving_track()
{
	switch(PATH_NOW.now_path)
  {
		case stanby:
			ROBOT_NOW.Vx=0;
		  ROBOT_NOW.Vy=0;
			break;
		case calibration://矫正初始角
			ROBOT_NOW.target_Vy=-40;
		  if(ROBOT_NOW.Vy>=-35)
			{
				MPU_all_init();
				ROBOT_CALIBRATION=CALIBRATION_SUCCEED;
				
			}
			break;
		case start_to_middle://开始到中间
			start_to_middle_moving(50,30,3);
			
			break;
		case middle_to_grab://中间到物料台前准备抓取
		{
			u16 delay1=500;
			u16 delay2=700;
			switch(PATH_NOW.next_location)
			{
				case path_right:	
					middle_to_grab_moving(30,-30,delay1,delay2,path_right);//vx vy delay1 delay2
				  break;
				case path_left:
				  middle_to_grab_moving(-30,-30,delay1,delay2,path_left);
				  break;
				case path_middle:
					middle_to_grab_moving(0,-30,0,delay2,path_middle);
					break;
			}	
		}
		break;
		case grab_to_put:
		{
			u8 v1=50;//高速速度
		  u8 v2=30;//低速速度
			u16 delay1=500;
			switch(PATH_NOW.now_location)
			{
				case path_left:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							grap_to_put_moving(0,-v1,v2,delay1,path_left);//black_count vhigh vlow
							break;
						case path_right:
							grap_to_put_moving(1,v1,v2,delay1,path_right);
							break;
						case path_middle:
							grap_to_put_moving(0,v1,v2,delay1,path_middle);
							break;
					}
					break;
				case path_right:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							grap_to_put_moving(1,-v1,v2,delay1,path_left);
							break;
						case path_right:
							grap_to_put_moving(0,v1,v2,delay1,path_right);
							break;
						case path_middle:
							grap_to_put_moving(0,-v1,v2,delay1,path_middle);
							break;
					}
				case path_middle:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							grap_to_put_moving(0,-v1,v2,delay1,path_left);
							break;
						case path_right:
							grap_to_put_moving(1,v1,v2,delay1,path_right);
							break;
						case path_middle:
							grap_to_put_moving(0,-v1,v2,delay1,path_middle);
							break;
					}
					break;
			}
		}
		break;
		case return_to_grab:
		{
			u8 v1=50;
			u8 v2=30;
			u8 v3=30;
			u8 v4=30;
			u32 delay3=500;
			u32 delay4=1200;
			switch(PATH_NOW.now_location)
			{
				case path_left:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							//回到物料台抓取 v1为水平移动高速速度 v2水平移动低速速度 count操作要压哪根线 v3水平速度 v4竖直速度 delay3水平延时 delay4竖直延时
							return_to_grab_moving(v1,v2,0,v3,-v4,delay3,delay4,path_left);
						case path_right:
							return_to_grab_moving(v1,v2,1,v3,-v4,delay3,delay4,path_right);
						case path_middle:
							return_to_grab_moving(v1,v2,1,0,-v4,0,delay4-delay3,path_middle);
					}
				case path_right:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							//回到物料台抓取 v1为水平移动高速速度 v2水平移动低速速度 count操作要压哪根线 v3水平速度 v4竖直速度 delay3水平延时 delay4竖直延时
							return_to_grab_moving(-v1,v2,1,-v3,-v4,delay3,delay4,path_left);
						case path_right:
							return_to_grab_moving(-v1,v2,0,-v3,-v4,delay3,delay4,path_right);
						case path_middle:
							return_to_grab_moving(-v1,v2,1,0,-v4,0,delay4-delay3,path_middle);
					}
				case path_middle:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							//回到物料台抓取 v1为水平移动高速速度 v2水平移动低速速度 count操作要压哪根线 v3水平速度 v4竖直速度 delay3水平延时 delay4竖直延时
							return_to_grab_moving(v1,v2,0,-v3,-v4,delay3,delay4,path_left);
						case path_right:
							return_to_grab_moving(v1,v2,0,v3,-v4,delay3,delay4,path_right);
						case path_middle:
							return_to_grab_moving(v1,v2,0,0,-v4,0,delay4-delay3,path_middle);
					}
			}
		}
		break;
		case return_to_start:
		{
			u8 v1=50;
			u8 v2=30;
			u8 v3=30;
			u8 v4=30;
			u32 delay3=500;
			u32 delay4=1200;
			switch(PATH_NOW.now_location)
			{
				case path_left:
						return_to_start_moving(-v1,v2,2,-v3,-v4,delay3,delay4,path_left);
				case path_right:
					  return_to_start_moving(-v1,v2,3,-v3,-v4,delay3,delay4,path_left);
				case path_middle:
						return_to_start_moving(-v1,v2,4,-v3,-v4,delay3,delay4,path_left);
			}
		}
		break;
	}
}

//延时移动(不影响主程序的延时)
u8 task_delay_ms(u32 delay_time)
{
	if(time.taskcount-time.taskcount_store>=delay_time)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//右边传感器双线循迹跟踪 v1移动方向和速度大小  v2矫正速度大小
u8 right_double_online_track(u8 v1,u8 v2)
{
	if((feedback.infrared_value&0x07)==0x00)
	{
		ROBOT_NOW.target_Vy=v1;
	}
	else if((feedback.infrared_value&0x07)==0x01)//需前移
	{
		ROBOT_NOW.target_Vy=v2;
	}
	else if((feedback.infrared_value&0x07)==0x04)//需后移
	{
		ROBOT_NOW.target_Vy=-v2;
	}
	else if((feedback.infrared_value&0x07)==0x02)//压线居中
	{
		ROBOT_NOW.target_Vy=0;
		return 1;
	}
	else if((feedback.infrared_value&0x07)==0x07)//全压线
	{
		ROBOT_NOW.target_Vy=0;
		return 2;
	}
	return 0;
}

//后面传感器单线循迹跟踪
u8 behind_online_track(u8 v1,u8 v2)
{
	if((feedback.infrared_value&0x70)==0x00)
	{
		ROBOT_NOW.target_Vx=v1;
	}
	else if((feedback.infrared_value&0x70)==0x10)//需右移
	{
		ROBOT_NOW.target_Vx=v2;
	}
	else if((feedback.infrared_value&0x70)==0x40)//需左移
	{
		ROBOT_NOW.target_Vx=-v2;
	}
	else if((feedback.infrared_value&0x70)==0x20)//压线
	{
		ROBOT_NOW.target_Vx=0;
		return 1;
	}
	else if((feedback.infrared_value&0x70)==0x70)//全压线
	{
		ROBOT_NOW.target_Vx=0;
		return 2;
	}
	return 0;
}

//右边传感器过线次数跟踪(最上靡桓鲦传感器判断过线次数)
u8 right_count_black_track(u8 count)
{
	if((feedback.infrared_value&0x08)==0x08)
	{
		feedback.right_black_count++;
	}
	if(feedback.right_black_count>=count)
		{
			return 1;
		}
	else{return 0;}
}

//后面传感器过线次数跟踪（最左边一个传感器判断过线次数）
u8 behind_count_black_track(u8 count)
{
	if((feedback.infrared_value&0x80)==0x80)
	{
		feedback.behind_black_count++;
		
	}
	if(feedback.behind_black_count>=count)
		{
			return 1;
		}
	else{return 0;}
}


//开始双线到中间 v1高速速度 v2低速速度
void start_to_middle_moving(u8 v1,u8 v2,u8 count)
{
	if(right_double_online_track(v1,v2))
	{
		if(behind_count_black_track(count))
		{
			if(behind_online_track(v1,v2))
			{
				ROBOT_TRACKING=TRACKING_SUCCEED;
				feedback.behind_black_count=0;
			}
		}
		else
		{
			ROBOT_NOW.target_Vx=50;
		}
	}
}

//middle_to_grab路段封装函数 vx为x轴速度 vy为y轴速度 delay1为x轴位移延时 delay2为y轴位移延时
void middle_to_grab_moving(u8 vx,u8 vy,u16 delay1,u16 delay2,enum LOCATION_PATH target)
{
	if(task_delay_ms(delay1))
	{
		ROBOT_NOW.target_Vx=0;
		if(task_delay_ms(delay1+delay2))
		{
			MPU_all_init();//重新初始化陀螺仪
			
			ROBOT_TRACKING=TRACKING_SUCCEED;//完成路段标志设置
			PATH_NOW.now_location=target;//记录当前位置
		}
		else{ROBOT_NOW.target_Vy=vy;}
	}
	else{ROBOT_NOW.target_Vx=vx;}
}
	
//grap_to_put路段规划封装函数 count为黑线条数的大小 v1为高速大小 v2为低速大小 
void grap_to_put_moving(u8 count,u8 v1,u8 v2,u16 delay1,enum LOCATION_PATH target)
{
	if(behind_count_black_track(count))
	{
		if(behind_online_track(v1,v2))
		{
			if(task_delay_ms(delay1))
			{
				ROBOT_NOW.target_Vy=0;
				if(behind_online_track(v1,v2)==1)
				{
					ROBOT_TRACKING=TRACKING_SUCCEED;//完成路段标志设置
					PATH_NOW.now_location=target;//记录当前位置
				}
			}
			else{ROBOT_NOW.target_Vy=30;}
		}
	}
	else{ROBOT_NOW.target_Vy=v1;}
}

//回到物料台抓取 v1为水平移动高速速度 v2水平移动低速速度 count操作要压哪根线 v3水平速度 v4竖直速度 delay3水平延时 delay4竖直延时
void return_to_grab_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u16 delay3,u16 delay4,enum LOCATION_PATH target)
{
	if(behind_count_black_track(2))//第一段,高速
	{
		if(right_count_black_track(4))//第二段,低速
		{
			if(right_double_online_track(30,30))//回归双线
			{
				start_to_middle_moving(v1,v2,count);//操作要压哪根线
				middle_to_grab_moving(v3,v4,delay3,delay4,target);//延时移动
			}
		}
		else{ROBOT_NOW.target_Vy=-30;}
	}
	else{ROBOT_NOW.target_Vy=-50;}
}

//回到起始区 v1为水平移动高速速度 v2水平移动低速速度 count操作要压哪根线 v3水平速度 v4竖直速度 delay3水平延时 delay4竖直延时
void return_to_start_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u16 delay3,u16 delay4,enum LOCATION_PATH target)
{
	return_to_grab_moving(v1,v2,count,v3,v4,delay3,delay4,target);
}


