#include "feedback.h"
#include "path.h"
#include "pid.h"
#include "math.h"
/***********定义区************/
struct FEEDBACK feedback={0,0,0,0,0,0};

PID pathover_pid;
static u8 start_calibration=0;

/***************反馈更新小车的信息********************/

//反馈得到机器人的世界轴实际速度
//轮子速度转换成世界坐标速度(求逆,或者写出v与小车坐标轴的矩阵再通过旋转变换得到)(前者耗cpu,后者人工算出结果矩阵即可)
void wheelvelocity_to_coordinateAxis_changeover(void)//coordinate Axis:坐标轴
{
	float thetainit=ROBOT_NOW.initial_yaw*pi/180.0f;//三角函数用的是弧度制
	float theta=(ROBOT_NOW.yaw*pi/180.0f)-thetainit;
	ROBOT_NOW.Vx=(-cos(theta))*encoder.v[0]+(1.0f/2*cos(theta)+sqrt(3)/2.0f*sin(theta))*encoder.v[1]+(1.0f/2*cos(theta)-sqrt(3)/2.0f*sin(theta))*encoder.v[2];
	ROBOT_NOW.Vy=sin(theta)*encoder.v[0]+(-1.0f/2*sin(theta)+sqrt(3)/2.0f*cos(theta))*encoder.v[1]+(-1.0f/2*sin(theta)-sqrt(3)/2.0f*cos(theta))*encoder.v[2];
	//ROBOT_NOW.w=L*(encoder.v[0]+encoder.v[1]+encoder.v[2]);
}


//后退顶墙校准(校准陀螺仪参照值)
void step_back_wall_calibration(void)
{
	//顶墙体校准(开始直接后退)//校准陀螺仪基准
	 if(ROBOT_CALIBRATION==CALIBRATION_WALL)
	 {
		 ROBOT_NOW.target_Vy=-100;

		 if(feedback.control_calibration==1)
     {
			 feedback.count_time=time.taskcount;
			 feedback.control_calibration=0;
		 }
		 else if((time.taskcount-feedback.count_time)>=1000&&time.taskcount!=0)//后退1秒(前面不开后面就不开，条件开关)
     {
			 ROBOT_CALIBRATION=CALIBRATION_SUCCEEDED;
		 }
	 }
	 if(ROBOT_CALIBRATION==CALIBRATION_SUCCEEDED)
	 {
		 ROBOT_NOW.target_Vy=0;
		 if(start_calibration!=0)
		 {  
			 ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw;
		 }
		 else if(start_calibration==0)
     {
			 ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw+pi;
			 start_calibration+=1;
		 }
	 }
 }

//黑线校准
void black_calibration(void)
{
		//根据黑线校准
	if(ROBOT_CALIBRATION==CALIBRATION_BLACK)
	{
		u8 counter_clockwise,clockwise;
	  if((feedback.trace_infrared&0x1f)==0x1f)ROBOT_NOW.target_Vy=80;
	  else
	  {
	   ROBOT_NOW.target_Vy=0;
	   if(((feedback.trace_infrared&0x1f)>>3)<3)clockwise=1;//左边压线
	   else if((feedback.trace_infrared&0x03)<3)counter_clockwise=1;//右边压线
	
	   if((feedback.trace_infrared&0x1f)==0x00)
	   {
	   	 feedback.black_calibration=0;
       //初始的坐标赋值
	   }
	   else if(clockwise&(!counter_clockwise))ROBOT_NOW.w=-10;
	   else if((!clockwise)&counter_clockwise)ROBOT_NOW.w=10;
     }
   }
}

//路径终点校准函数
void pathover_calibration(void)
{
	float k3;
	float direct_unit_vector_x;
	float direct_unit_vector_y;
	float V_direct;
	float s;
	if(ROBOT_CALIBRATION==CALIBRATION_PATHOVER)
  {
	if(ROBOT_NOW.X!=PATH_NOW.X1)
  {
		k3=(ROBOT_NOW.Y-PATH_NOW.Y1)/(ROBOT_NOW.X-PATH_NOW.X1);
		direct_unit_vector_x=sqrt(1.0f/(k3*k3+1.0f));
		direct_unit_vector_y=direct_unit_vector_x*k3;
		
	}
	else //斜率不存在
	{
		direct_unit_vector_x=0;
		direct_unit_vector_y=1.0f;
	}
	
	s=sqrt((PATH_NOW.Y1-PATH_NOW.Y0)*(PATH_NOW.Y1-PATH_NOW.Y0)+(PATH_NOW.X1-PATH_NOW.X0)*(PATH_NOW.X1-PATH_NOW.X0));//现在的坐标到路径终点的距离
	
	//pid调控数据获取(仅通过pid调控获得补偿量大小,方向由单位分向量标定)
	pid_position_calculate_error(&pathover_pid,s);
	V_direct=pathover_pid.output;
	
	//修正V_direct符号(速度控符号)(通过与终点比较标定)
	if(ROBOT_NOW.Y>PATH_NOW.Y1)direct_unit_vector_y=-direct_unit_vector_y;//一二象限标负
	if(ROBOT_NOW.X>PATH_NOW.X1)direct_unit_vector_x=-direct_unit_vector_x;//一四象限标负
	
	
	//设置目标速度
	ROBOT_NOW.target_Vx=V_direct*direct_unit_vector_x;
	ROBOT_NOW.target_Vy=V_direct*direct_unit_vector_y;//pid调控
	
	}
}


	//关于获得robot的x和y在世界坐标系的坐标
/*我想到有如下设计方案
	
	1，校准定位后，通过编码器计算y轴，x轴用激光，再通过黑线反馈矫正
	2，x，y轴都用激光，关于y轴突变，可做补偿处理物料台的凸出，可行性比较高。(同样通过黑线反馈矫正)
	3，用定时器(1ms)跟速度做运算，跟踪算出y轴坐标，再通过黑线反馈矫正
	经思考，2方案比较可行，暂用二方案
*/


//激光测距获得坐标，通过地面黑线反馈校准
//陀螺仪获得角度和角速度
void update_robot_state(void)
{
	float yaw;
  mpu_dmp_get_yaw_data(&yaw);
	ROBOT_NOW.yaw=yaw;
	ROBOT_NOW.w=get_yaw_angular_velocity();

	vl53l0x_general_start(dev,0);//获得激光测距数据(x坐标)mm
	//关于y坐标，，，暂待
	
	
}



//转身模式中断函数
void turnaround_interrupt(enum ROBOT_STATE_TYPE LAST)
{
	LAST_STATE=LAST;
	ROBOT_STATE=TURNAROUND;
  ROBOT_TURNAROUND=TURNAROUND_IT;
}


//校准模式中断函数
void calibration_interrupt(enum ROBOT_STATE_TYPE LAST,enum CALIBRATION_TYPE TYPE)
{
	LAST_STATE=LAST;
	ROBOT_STATE=CALIBRATION;
	ROBOT_CALIBRATION=TYPE;
}

