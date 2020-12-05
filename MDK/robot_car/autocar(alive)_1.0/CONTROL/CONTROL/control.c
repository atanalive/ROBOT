#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "motor.h"
#include "fsm.h"

/************alive*************/
PID moto_pid;
PID wheel_pid;
 
 
void control_encoder_init(void)
{
	pidinit(&moto_pid,0,0,0,1000,1000,5,PLUS);
	pidinit(&wheel_pid,0,0,0,1000,1000,5,EQUAL);
	for(u8 h;h<3;h++)
	{
	  encoder.target_velocity[h]=1000;
    encoder.target_location[h]=10000;
  }
}	
/********传入目标值和状态值的pid********/
//PID电机速度控制
void motor_velocity_control(void)//A B C 三个轮子分别对应3,4,5
{
	for(u8 i=0;i<3;i++)
	{
		pid_position_calculate(&moto_pid,encoder.v[i],encoder.target_velocity[i]);
    set_motor(i,moto_pid);
	}
}


//PID轮子路程控制
void wheel_journey_control(u8 i)//A B C 三个轮子分别对应3,4,5
{ 
	int encoder_data[3];
	
		switch(i)
    {
			case 3:
				encoder_data[i-3]=encoder.s[0];
			case 4:	
				encoder_data[i-3]=encoder.s[1];
			case 5:	
				encoder_data[i-3]=encoder.s[2];
		}
		pid_position_calculate(&wheel_pid,encoder_data[i-3],encoder.target_location[i-3]);
    set_motor(i,wheel_pid);
}


//操控电机
void set_motor(u8 i,PID motor_pid)
{
	switch(i)
	{
		case 3:
      if(moto_pid.output>0){AIN1=1;AIN2=0;}
      else if(moto_pid.output<0){AIN1=0;AIN2=1;}			
	   	PWMA=abs_num(motor_pid.output);
		  break;
    case 4:
			if(moto_pid.output>0){BIN1=1;BIN2=0;}
      else if(moto_pid.output<0){BIN1=0;BIN2=1;}		
			PWMB=abs_num(motor_pid.output);
		  break;
		case 5:
			if(moto_pid.output>0){CIN1=1;CIN2=0;}
      else if(moto_pid.output<0){CIN1=0;CIN2=1;}		
			PWMC=abs_num(motor_pid.output);
		  break;
	}
}

u16 abs_num(float a)
{
	if(a>0)a=a;
	else a=-a;
	return a;
}



