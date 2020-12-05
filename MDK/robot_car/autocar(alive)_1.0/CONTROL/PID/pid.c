#include "pid.h"

//pid�ĳ�ʼ��
//������������Χ
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

//���Ʊ����ķ�Χ
float limit_value(float num,float limit)
{
	if(num>limit)num=limit;
	else if(num<-limit)num=-limit;
	
	return num;
}

/********����Ŀ��ֵ��״ֵ̬��pid********/
//λ��ʽpid����
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
	if(pid->mode)//����ֱ�ӿ���ͨ����ֵ�������������ٶȵĲ���
	{
	  pid->output+=pid->delta_e;
	}
	else if(!pid->mode)//�����Ӳ�����������λ�õĲ���
  {
		pid->output=pid->delta_e;
	}
	
}

/***************���������pid***************/
//λ��ʽ
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
	if(pid->mode)//����ֱ�ӿ���ͨ����ֵ�������������ٶȵĲ���
	{
	  pid->output+=pid->delta_e;
	}
	else if(!pid->mode)//�����Ӳ�����������λ�õĲ���
  {
		pid->output=pid->delta_e;
	}
	
}


