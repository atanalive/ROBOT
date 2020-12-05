#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "motor.h"
#include "fsm.h"

/***************************************
使用A4950驱动模块
引脚输入: <1>两个PWM  <2>PWM+IO口输出
***************************************/


/************alive*************/
PID moto_pid;
PID wheel_pid;
 

void control_encoder_init(void)
{
	//pid p i d outmax integralmax deadzone
	pidinit(&moto_pid,50,1.0,2.0,1000,1000,10);
}	

/********传入目标值和状态值的pid********/
//PID电机速度控制
void motor_velocity_control(void)//A B C 三个轮子分别对应3,4,5
{
	for(u8 i=0;i<3;i++)
	{
		pid_position_calculate(&moto_pid,encoder.v[i],encoder.target_velocity[i]);
    set_motor(i+3,moto_pid);
	}
}

//操控电机
void set_motor(u8 i,PID motor_pid)
{
	switch(i)
	{
		case 3:
      if(moto_pid.output>0){AIN1=1;AIN2=0;}
      else if(moto_pid.output<0){AIN1=0;AIN2=1;}
      else{AIN1=1;AIN2=1;}			
	   	PWMA+=abs_num(motor_pid.output);
		  break;
    case 4:
			if(moto_pid.output>0){BIN1=1;BIN2=0;}
      else if(moto_pid.output<0){BIN1=0;BIN2=1;}
			else{BIN1=1;BIN2=1;}			
			PWMB+=abs_num(motor_pid.output);
		  break;
		case 5:
			if(moto_pid.output>0){CIN1=1;CIN2=0;}
      else if(moto_pid.output<0){CIN1=0;CIN2=1;}
			else{CIN1=1;CIN2=1;}			
			PWMC+=abs_num(motor_pid.output);
		  break;
	}
}

u16 abs_num(float a)
{
	if(a>0)a=a;
	else a=-a;
	return a;
}



