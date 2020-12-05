#include "control.h"
#include "pid.h"
#include "encoder.h"
#include "motor.h"
#include "sys.h"
#include "usart.h"
#include "robot.h"
#include "main.h"
#define stand_anger 0 //ֱ���Ƕ� �ȼ���Ϊ0
/***************************************
ʹ��tb6612����ģ��
��������: <1>����PWM  <2>PWM+IO�����
***************************************/
#define ZHONGZHI 3

/************alive*************/

PID stand_pid = {0};
#if FUNC_VELOCITY
PID velocity_pid[2]={{0},{0}};
#endif
float Balance_Kp=300,Balance_Kd=1,Velocity_Kp=80,Velocity_Ki=0.4;//PID����
int Encoder_Left,Encoder_Right;             //���ұ��������������

//����pid��ʼ��
void pid_all_init(void)
{
	pidinit(&stand_pid,40,0,5,3500,0,10);
	
	//pid p i d outmax integralmax deadzone
	#if FUNC_VELOCITY
	pidinit(&velocity_pid[0],40,0.6,20,3500,700,10);//����pid
	pidinit(&velocity_pid[1],40,1.0,30,3500,1000,10);//����pid
	#endif
}	

/********����Ŀ��ֵ��״ֵ̬��pid********/
//PID����ٶȿ���
void motor_velocity_control(void)//A B C �������ӷֱ��Ӧ3,4,5
{
	u8 i;
	float output[2]={0};
	//ֱ����pid
	pid_position_calculate(&stand_pid,robot.anger,stand_anger);
	#if FUNC_VELOCITY
	//�ٶȻ�pid
	for(i=0;i<2;i++)
	{
		pid_position_calculate(&velocity_pid[i],robot.wheel_v[i],robot.target_wheel_v[i]);
	}
	#endif
	//����pwm
	for(i=0;i<2;i++)
	{
		output[i]+=stand_pid.output;//ֱ����pid����
		#if FUNC_VELOCITY
		output[i]+=velocity_pid[i].output;//�ٶȻ�pid����
		#endif
		output[i]=output_limit(output[i],3000);
    set_motor(i+3,output[i]);
	}
	
}

//�ٿص��
void set_motor(u8 i,float output)
{
	//printf("�ٿص��");
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
//output����ֵ������
float output_limit(float output,float limit)
{
	if(output>limit)output=limit;
	else if(output<-limit)output=-limit;
	return output;
}


/**************************************************************************
�������ܣ�ֱ��PD����
��ڲ������Ƕȡ����ٶ�
����  ֵ��ֱ������PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int balance(float Angle,float Gyro)
{  
   float Bias;
	 int balance;
	 Bias=Angle-ZHONGZHI;       //===���ƽ��ĽǶ���ֵ �ͻ�е���
	 balance=Balance_Kp*Bias+Gyro*Balance_Kd;   //===����ƽ����Ƶĵ��PWM  PD����   kp��Pϵ�� kd��Dϵ�� 
	 return balance;
}

/**************************************************************************
�������ܣ��ٶ�PI���� �޸�ǰ�������ٶȣ�����Target_Velocity�����磬�ĳ�60�ͱȽ�����
��ڲ��������ֱ����������ֱ�����
����  ֵ���ٶȿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int velocity(int encoder_left,int encoder_right)
{  
     static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
   //=============�ٶ�PI������=======================//	
		Encoder_Least =(Encoder_Left+Encoder_Right)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                       //===����ң�������ݣ�����ǰ������
		if(Encoder_Integral>10000)  	Encoder_Integral=10000;             //===�����޷�
		if(Encoder_Integral<-10000)	Encoder_Integral=-10000;              //===�����޷�	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;                          //===�ٶȿ���	
	  return Velocity;
}
