#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
#include "robot.h"
#include "main.h"
#define stand_anger 0 //直立角度 先假设为0
/***************************************
使用tb6612驱动模块
引脚输入: <1>两个PWM  <2>PWM+IO口输出
***************************************/
#define ZHONGZHI 3

/************alive*************/

PID stand_pid = {0};
#if FUNC_VELOCITY
PID velocity_pid[2]={{0},{0}};
#endif
float Balance_Kp=300,Balance_Kd=1,Velocity_Kp=80,Velocity_Ki=0.4;//PID参数
int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数

//轮子pid初始化
void pid_all_init(void)
{
	pidinit(&stand_pid,40,0,5,3500,0,10);
	
	//pid p i d outmax integralmax deadzone
	#if FUNC_VELOCITY
	pidinit(&velocity_pid[0],40,0.6,20,3500,700,10);//左轮pid
	pidinit(&velocity_pid[1],40,1.0,30,3500,1000,10);//右轮pid
	#endif
}	

/********传入目标值和状态值的pid********/
//PID电机速度控制
void motor_velocity_control(void)//A B C 三个轮子分别对应3,4,5
{
	u8 i;
	float output[2]={0};
	//直立环pid
	pid_position_calculate(&stand_pid,robot.anger,stand_anger);
	#if FUNC_VELOCITY
	//速度环pid
	for(i=0;i<2;i++)
	{
		pid_position_calculate(&velocity_pid[i],robot.wheel_v[i],robot.target_wheel_v[i]);
	}
	#endif
	//设置pwm
	for(i=0;i<2;i++)
	{
		output[i]+=stand_pid.output;//直立环pid叠加
		#if FUNC_VELOCITY
		output[i]+=velocity_pid[i].output;//速度环pid叠加
		#endif
		output[i]=output_limit(output[i],3000);
    set_motor(i+3,output[i]);
	}
	
}

//操控电机
void set_motor(u8 i,float output)
{
	//printf("操控电机");
	switch(i)
	{
		case 3:
      if(output>0){AIN1=1;AIN2=0;}
      else if(output<0){AIN1=0;AIN2=1;}
      else{AIN1=1;AIN2=1;}			
	   	PWMA=abs_num(output);
			//printf("PWMA is %f \r\n",motor_pid->output);
		  break; 
    case 4:
			if(output>0){BIN1=1;BIN2=0;}
      else if(output<0){BIN1=0;BIN2=1;}
			else{BIN1=1;BIN2=1;}
			PWMB=abs_num(output);
			//printf("PWMB is %f \r\n",motor_pid->output);
		  break;
	}
}

u16 abs_num(float a)
{
	if(a>0)a=a;
	else a=-a;
	return a;
}
//output绝对值的限制
float output_limit(float output,float limit)
{
	if(output>limit)output=limit;
	else if(output<-limit)output=-limit;
	return output;
}


/**************************************************************************
函数功能：直立PD控制
入口参数：角度、角速度
返回  值：直立控制PWM
作    者：平衡小车之家
**************************************************************************/
int balance(float Angle,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-ZHONGZHI;       //===求出平衡的角度中值 和机械相关
	 balance=Balance_Kp*Bias+Gyro*Balance_Kd;   //===计算平衡控制的电机PWM  PD控制   kp是P系数 kd是D系数 
	 return balance;
}

/**************************************************************************
函数功能：速度PI控制 修改前进后退速度，请修Target_Velocity，比如，改成60就比较慢了
入口参数：左轮编码器、右轮编码器
返回  值：速度控制PWM
作    者：平衡小车之家
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
     static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
   //=============速度PI控制器=======================//	
		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.8;		                                                //===一阶低通滤波器       
		Encoder += Encoder_Least*0.2;	                                    //===一阶低通滤波器    
		Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===接收遥控器数据，控制前进后退
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===积分限幅
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===积分限幅	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //===速度控制	
	  return Velocity;
}
