#include "main.h"
#include "math.h"
#include "moving.h"
#include "path.h"
#include "fsm.h"
#include "usart.h"
#include "encoder.h"
#include "feedback.h"

struct MOVING_CONTROL moving={480,200,0,150,150,768,768};
float theta;
static u8 a=1;
PID line_pid;
PID whirling_pid;


//世界坐标速度转换成轮子速度
void coordinates_to_wheelvelocity_changeover(void)
{
	if(a==1){ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw;a++;}//得到初始角度
  theta=pi*(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw)/180.0f;//转为弧度制，我将pi定义在main.h了
	
	encoder.target_velocity[0]=sin(theta)*ROBOT_NOW.target_Vx-cos(theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[1]=sin(pi/3-theta)*ROBOT_NOW.target_Vx+cos(pi/3-theta)+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[2]=(-sin(pi/3+theta))*ROBOT_NOW.target_Vx+cos(pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
}

//改良了师兄的路径跟踪算法，去掉了不需要的部分并优化，效果有待测试
//路径跟踪(转身，梯子加减速，匀速)
//反馈标定arrived
//直线路径跟踪(实现始终指向目标,再用pid调控归线)
void moving_line_track(void)
{
	float error;
	float vertical_unit_vector_x;//与线垂直方向单位向量在x轴的分量
	float vertical_unit_vector_y;//与线垂直方向单位向量在y轴的分量
	float direct_unit_vector_x;//指向终点的单位向量在x轴的分量
	float direct_unit_vector_y;//指向终点的单位向量在y轴的分量
	//float V_vertical;//与线垂直方向的速度
	float V_direct;//指向终点的速度
	float V_parallel;//跟线平行方向的速度
	float k1;//路径斜率
	float b1;//路径直线截距
	float k2;//垂直路径方向的斜率
	float k3;//指向终点的直线的斜率
	float d;//与终点的直线距离
	float b;//跟终点的直线距离在路径上的分量
	float s;//该路径总长度
	float pid_control;
	//pid参数初始化
	pidinit(&line_pid,0,0,0,100,100,5,EQUAL);//参数有待调整
	//计算机器人垂直路径方向的误差与单位向量
	if(PATH_NOW.X0!=PATH_NOW.X1)
  {
		k1=(PATH_NOW.Y1-PATH_NOW.Y0)/(PATH_NOW.X1-PATH_NOW.X0);
		b1=PATH_NOW.Y0-k1*PATH_NOW.X0;
		if(k1!=0)//斜率不为0
    {
			k2=-1.0f/k1;
			vertical_unit_vector_x=sqrt(1.0f/(k2*k2+1));
			vertical_unit_vector_y=k2*vertical_unit_vector_x;
			error=float_abs(-k1*ROBOT_NOW.X+ROBOT_NOW.Y)/sqrt(k1*k1+1.0f);
			//修正符号(通过操作单位向量的分量)(通过k2斜率及误差点相对大小标定)
			if(ROBOT_NOW.Y>=k1*ROBOT_NOW.X+b1)
			{
				vertical_unit_vector_y=-vertical_unit_vector_y;
				if(k2>0)vertical_unit_vector_x=-vertical_unit_vector_x;
			}
			else
      {
				if(k2<0)vertical_unit_vector_x=-vertical_unit_vector_x;
			}
		}
		else//若路径斜率为0
    {
			vertical_unit_vector_x=0;
			vertical_unit_vector_y=1.0f;
			error=PATH_NOW.Y0-ROBOT_NOW.Y;
			if(error<0)
			{
				vertical_unit_vector_y=-vertical_unit_vector_y;
			}
		}
	}
	else //如果路径斜率不存在
	{
		vertical_unit_vector_x=1.0f;
		vertical_unit_vector_y=0;
		error=PATH_NOW.X0-ROBOT_NOW.X;
		if(error<0)
			{
				vertical_unit_vector_x=-vertical_unit_vector_x;
			}
	}
	
	//速度规划需要的量
	//目前位置到路径终点的欧式距离
	d=sqrt((ROBOT_NOW.X-PATH_NOW.X1)*(ROBOT_NOW.X-PATH_NOW.X1)+(ROBOT_NOW.Y-PATH_NOW.Y1)*(ROBOT_NOW.Y-PATH_NOW.Y1));
	b=sqrt(d*d-error*error);	//d沿路径方向的分量
	s=sqrt((PATH_NOW.Y1-PATH_NOW.Y0)*(PATH_NOW.Y1-PATH_NOW.Y0)+(PATH_NOW.X1-PATH_NOW.X0)*(PATH_NOW.X1-PATH_NOW.X0));//现在的坐标到路径终点的距离
	
	if(s<5){calibration_interrupt(ROBOT_STATE,CALIBRATION_PATHOVER);return;}//判断是否到终点
	
	//规划平行速度
	V_parallel=moving_line_velocity_planning_trapezoid(b,s);
	

	
	//机器人目前的位置和终点的连线
	//如果斜率存在
	if(ROBOT_NOW.X!=PATH_NOW.X1)
  {
		k3=(ROBOT_NOW.Y-PATH_NOW.Y1)/(ROBOT_NOW.X-PATH_NOW.X1);
		direct_unit_vector_x=sqrt(1.0f/(k3*k3+1.0f));
		direct_unit_vector_y=direct_unit_vector_x*k3;
		
		V_direct=sqrt((error*V_parallel/b)*(error*V_parallel/b)+V_parallel*V_parallel);
		
	}
	else //斜率不存在
	{
		direct_unit_vector_x=0;
		direct_unit_vector_y=1.0f;
		
		V_direct=sqrt((error*V_parallel/b)*(error*V_parallel/b)+V_parallel*V_parallel);
	}
	
	//pid调控归线数据获取(仅通过pid调控补偿量大小,方向由单位分向量标定)
	pid_position_calculate_error(&line_pid,float_abs(error));
	pid_control=line_pid.output;
	
	//修正V_direct符号(速度控符号)(通过与终点比较标定)
	if(ROBOT_NOW.Y>PATH_NOW.Y1)direct_unit_vector_y=-direct_unit_vector_y;//一二象限标负
	if(ROBOT_NOW.X>PATH_NOW.X1)direct_unit_vector_x=-direct_unit_vector_x;//一四象限标负
	
	
	//设置目标速度
	ROBOT_NOW.target_Vx=pid_control*vertical_unit_vector_x+V_direct*direct_unit_vector_x;
	ROBOT_NOW.target_Vy=pid_control*vertical_unit_vector_y+V_direct*direct_unit_vector_y;
	
}

//沿路径线速度规划--梯形加减速
float moving_line_velocity_planning_trapezoid(float b,float s)
{
	float set_v=0.0f;
	float traveled_s;
	
	traveled_s=s-b;//计算平行方向走过的路程
	
	//低速匀速行驶模式
	if(moving.V_low_mode==1)
  {
		return set_v=moving.V_low;
	}
	
	if(s<moving.s_slow_down+moving.s_speed_up)
  {
		printf("s is shorter than s_slow_down+moving.s_speed_up!!!\r\n");
    return set_v=moving.V_low;
	};
	
	if(traveled_s<moving.s_speed_up)//加速阶段(严格控制速度爬升,在traveled_s=moving.s_speed_up时达到最大速度)
		set_v=sqrt(moving.V_max*moving.V_max-2.0f*moving.a1*(moving.s_speed_up-traveled_s));//v0*v0=vt*vt-2as(将vt更改为v0作为输出,更利于速度的控制)
	else if(traveled_s>moving.s_speed_up&&traveled_s<(s-moving.s_slow_down))//匀速阶段
	  set_v=moving.V_max;
	else if(b<moving.s_slow_down)//减速阶段
		set_v=sqrt(moving.V_max*moving.V_max-2.0f*moving.a2*(moving.s_slow_down-b));
	else
	{
		printf("moving_line_velocity_planning_trapezoid is unnormal!!!\r\n");
	  return set_v=moving.V_low;
	}
	return set_v;
}

//取float类型的绝对值
float float_abs(float value)
{
	if(value<0)
  {
		value=-value;
	}
	return value;
}



//移动or转身(涉及二重中断处理)
void go_straight_or_turn_around(void)
{
if(ROBOT_TURNAROUND==TURNAROUND_STAYBACK)
{
	static u8 prevent_repeat=0;//防止重复进入后退转身程序
	if(prevent_repeat==0)
  { 
	
		if(!(((feedback.trace_infrared&0x1f)>>3)<0x03&&(feedback.trace_infrared&0x03)<0x03))//只要不是两边都压线
		{
		  if(ROBOT_STATE==TURNAROUND_TO_PUT)
		  {
        ROBOT_NOW.target_Vy=100;			
      }
		  else if(ROBOT_STATE==TURNAROUND_TO_TABLE)
		  {
			  ROBOT_NOW.target_Vy=-100;
		  }
  	}
		else //两边都压线
		{
       ROBOT_NOW.target_Vy=0;
       turnaround_interrupt(ROBOT_STATE);
			 prevent_repeat=1;
		}
	}
	if(prevent_repeat==1&&BEFORE_STATE==TURNAROUND_TO_PUT)//后退校准
	{
		calibration_interrupt(ROBOT_STATE,CALIBRATION_WALL);
    prevent_repeat=2;
		return;
	}
	else if(prevent_repeat==1&&BEFORE_STATE!=TURNAROUND_TO_PUT)//如果不需要后退校准
  {
		ROBOT_TURNAROUND=TURNAROUND_SUCCEEDED;
		prevent_repeat=0;
	}
	if(prevent_repeat==2)
  {
		ROBOT_TURNAROUND=TURNAROUND_SUCCEEDED;
		prevent_repeat=0;
	}
}
}




//转身动作，逆时针180°
void whirling_180_angle(void)//以逆时针为正
{ 
	static u8 whirling_count=0;
	if(ROBOT_TURNAROUND==TURNAROUND_START|ROBOT_TURNAROUND==TURNAROUND_IT|ROBOT_TURNAROUND==TURNAROUND_STAYBACK)
	{		
		ROBOT_NOW.target_Vx=ROBOT_NOW.target_Vy=0.0f;
		pid_position_calculate(&whirling_pid,ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw,180);
	  ROBOT_NOW.target_w=whirling_pid.output;
		if(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw<1)whirling_count++;
		else whirling_count--;
		if(whirling_count>5)//数据比较稳定后标志完成(误差1°以下的时间大于1°以上的时间一定程度就标志完成)
    {
			ROBOT_TURNAROUND=TURNAROUND_SUCCEEDED;
			whirling_count=0;
		}
		
	}
}


