#include "pid.h"

//pid的初始化
//死区：允许误差范围
void pidinit(PID *pid,float kp,float ki,float kd,float outmax,float integralmax,float deadzone,u8 mode)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->deadzone=deadzone;
	pid->integral_limit=integralmax;
	pid->output_limit=outmax;
	pid->accumulate_e=pid->e=pid->last_e=pid->output=pid->pwm=0.0f;
	pid->mode=mode;
}

//限制变量的范围
float limit_value(float num,float limit)
{
	if(num>limit)num=limit;
	else if(num<-limit)num=-limit;
	
	return num;
}

/********传入目标值和状态值型pid********/
//位置式pid控制
void pid_position_calculate(PID *pid,int currentpoint,int nextpoint)
{
  
	if(pid->firstflag==1)
	{
		pid->last_e=nextpoint-currentpoint;
		pid->firstflag++;
	}
	else
  {		
		pid->last_e=pid->e;
	}
	pid->e=nextpoint-currentpoint;
	pid->accumulate_e+=pid->e;
	pid->delta_e=pid->kp*pid->e+limit_value(pid->ki*pid->accumulate_e,pid->integral_limit)+pid->kd*(pid->e-pid->last_e);
	if(pid->delta_e<pid->deadzone&&pid->delta_e>-pid->deadzone)
  {
		pid->delta_e=0;
	}
	if(pid->mode)//处理直接可以通过数值补偿的量，如速度的补偿
	{
	  pid->output+=pid->delta_e;
	}
	else if(!pid->mode)//处理间接补偿的量，如位置的补偿
  {
		pid->output=pid->delta_e;
	}
	
}

/***************传入误差型pid***************/
//位置式
void pid_position_calculate_error(PID *pid,float error)
{
  
	if(pid->firstflag==1)
	{
		pid->last_e=error;
		pid->firstflag++;
	}
	else
  {		
		pid->last_e=pid->e;
	}
	pid->e=error;
	pid->accumulate_e+=pid->e;
	pid->delta_e=pid->kp*pid->e+limit_value(pid->ki*pid->accumulate_e,pid->integral_limit)+pid->kd*(pid->e-pid->last_e);
	if(pid->delta_e<pid->deadzone&&pid->delta_e>-pid->deadzone)
  {
		pid->delta_e=0;
	}
	if(pid->mode)//处理直接可以通过数值补偿的量，如速度的补偿
	{
	  pid->output+=pid->delta_e;
	}
	else if(!pid->mode)//处理间接补偿的量，如位置的补偿
  {
		pid->output=pid->delta_e;
	}
	
}


