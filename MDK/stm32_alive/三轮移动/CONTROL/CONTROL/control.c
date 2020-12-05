#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
/***************************************
使用tb6612驱动模块
引脚输入: <1>两个PWM  <2>PWM+IO口输出
***************************************/


/************alive*************/
PID motor[3]={{0},{0},{0}};
 
//轮子pid初始化
void control_encoder_init(void)
{
	u8 i;
	for(i=0;i<3;i++)
	{
		//pid p i d outmax integralmax deadzone
		pidinit(&motor[i],40,0.6,20,3500,700,10);//轮子pid
	}
	
	
}	

/********传入目标值和状态值的pid********/
//PID电机速度控制
void motor_velocity_control(void)//A B C 三个轮子分别对应3,4,5
{
	u8 i;
	for(i=0;i<3;i++)
	{
		pid_position_calculate(&motor[i],encoder.v[i],encoder.target_velocity[i]);
    set_motor(i+3,&motor[i]);
	}
}

//操控电机
void set_motor(u8 i,PID *motor_pid)
{
	printf("操控电机");
	switch(i)
	{
		case 3:
      if(motor_pid->output>0){AIN1=1;AIN2=0;PWMA=motor_pid->output;}
      else if(motor_pid->output<0){AIN1=0;AIN2=1;PWMA=abs_num(motor_pid->output);}
      else{AIN1=1;AIN2=1;}			
	   	
			//printf("PWMA is %f \r\n",motor_pid->output);
		  break; 
    case 4:
			if(motor_pid->output>0){BIN1=1;BIN2=0;PWMB=motor_pid->output;}
      else if(motor_pid->output<0){BIN1=0;BIN2=1;PWMB=abs_num(motor_pid->output);}
      else{BIN1=1;BIN2=1;}
			//printf("PWMB is %f \r\n",motor_pid->output);
		  break;
		case 5:
			if(motor_pid->output>0){CIN1=1;CIN2=0;PWMC=motor_pid->output;}
      else if(motor_pid->output<0){CIN1=0;CIN2=1;PWMC=abs_num(motor_pid->output);}
      else{CIN1=1;CIN2=1;}
			//printf("PWMC is %f \r\n",motor_pid->output);
		  break;
	}
}

u16 abs_num(float a)
{
	if(a>0)a=a;
	else a=-a;
	return a;
}



