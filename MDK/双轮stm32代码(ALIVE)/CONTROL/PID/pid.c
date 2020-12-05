#include "pid.h"
#include "usart.h"
//pid�ĳ�ʼ��
//������������Χ
//pid p i d outmax integralmax deadzone
void pidinit(PID *pid,float kp,float ki,float kd,float outmax,float integralmax,float deadzone)
{
	pid->kp=kp;
	pid->ki=ki;
	pid->kd=kd;
	pid->deadzone=deadzone;
	pid->integral_limit=integralmax;
	pid->output_limit=outmax;
	pid->accumulate_e=pid->e=pid->last_e=pid->output=pid->pwm=0.0f;
	pid->firstflag=1;
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
	//printf("pid\r\n");
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
	//����ֱ�ӿ���ͨ����ֵ�������������ٶȵĲ���
	pid->output=pid->delta_e;
	pid->output=limit_value(pid->output,pid->output_limit);//����������Сֵ
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
	//����ֱ�ӿ���ͨ����ֵ�������������ٶȵĲ���
	pid->output+=pid->delta_e;
	pid->output=limit_value(pid->output,pid->output_limit);//����������Сֵ
}


