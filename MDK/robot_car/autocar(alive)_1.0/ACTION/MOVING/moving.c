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


//���������ٶ�ת���������ٶ�
void coordinates_to_wheelvelocity_changeover(void)
{
	if(a==1){ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw;a++;}//�õ���ʼ�Ƕ�
  theta=pi*(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw)/180.0f;//תΪ�����ƣ��ҽ�pi������main.h��
	
	encoder.target_velocity[0]=sin(theta)*ROBOT_NOW.target_Vx-cos(theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[1]=sin(pi/3-theta)*ROBOT_NOW.target_Vx+cos(pi/3-theta)+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[2]=(-sin(pi/3+theta))*ROBOT_NOW.target_Vx+cos(pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
}

//������ʦ�ֵ�·�������㷨��ȥ���˲���Ҫ�Ĳ��ֲ��Ż���Ч���д�����
//·������(ת�����ӼӼ��٣�����)
//�����궨arrived
//ֱ��·������(ʵ��ʼ��ָ��Ŀ��,����pid���ع���)
void moving_line_track(void)
{
	float error;
	float vertical_unit_vector_x;//���ߴ�ֱ����λ������x��ķ���
	float vertical_unit_vector_y;//���ߴ�ֱ����λ������y��ķ���
	float direct_unit_vector_x;//ָ���յ�ĵ�λ������x��ķ���
	float direct_unit_vector_y;//ָ���յ�ĵ�λ������y��ķ���
	//float V_vertical;//���ߴ�ֱ������ٶ�
	float V_direct;//ָ���յ���ٶ�
	float V_parallel;//����ƽ�з�����ٶ�
	float k1;//·��б��
	float b1;//·��ֱ�߽ؾ�
	float k2;//��ֱ·�������б��
	float k3;//ָ���յ��ֱ�ߵ�б��
	float d;//���յ��ֱ�߾���
	float b;//���յ��ֱ�߾�����·���ϵķ���
	float s;//��·���ܳ���
	float pid_control;
	//pid������ʼ��
	pidinit(&line_pid,0,0,0,100,100,5,EQUAL);//�����д�����
	//��������˴�ֱ·�����������뵥λ����
	if(PATH_NOW.X0!=PATH_NOW.X1)
  {
		k1=(PATH_NOW.Y1-PATH_NOW.Y0)/(PATH_NOW.X1-PATH_NOW.X0);
		b1=PATH_NOW.Y0-k1*PATH_NOW.X0;
		if(k1!=0)//б�ʲ�Ϊ0
    {
			k2=-1.0f/k1;
			vertical_unit_vector_x=sqrt(1.0f/(k2*k2+1));
			vertical_unit_vector_y=k2*vertical_unit_vector_x;
			error=float_abs(-k1*ROBOT_NOW.X+ROBOT_NOW.Y)/sqrt(k1*k1+1.0f);
			//��������(ͨ��������λ�����ķ���)(ͨ��k2б�ʼ�������Դ�С�궨)
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
		else//��·��б��Ϊ0
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
	else //���·��б�ʲ�����
	{
		vertical_unit_vector_x=1.0f;
		vertical_unit_vector_y=0;
		error=PATH_NOW.X0-ROBOT_NOW.X;
		if(error<0)
			{
				vertical_unit_vector_x=-vertical_unit_vector_x;
			}
	}
	
	//�ٶȹ滮��Ҫ����
	//Ŀǰλ�õ�·���յ��ŷʽ����
	d=sqrt((ROBOT_NOW.X-PATH_NOW.X1)*(ROBOT_NOW.X-PATH_NOW.X1)+(ROBOT_NOW.Y-PATH_NOW.Y1)*(ROBOT_NOW.Y-PATH_NOW.Y1));
	b=sqrt(d*d-error*error);	//d��·������ķ���
	s=sqrt((PATH_NOW.Y1-PATH_NOW.Y0)*(PATH_NOW.Y1-PATH_NOW.Y0)+(PATH_NOW.X1-PATH_NOW.X0)*(PATH_NOW.X1-PATH_NOW.X0));//���ڵ����굽·���յ�ľ���
	
	if(s<5){calibration_interrupt(ROBOT_STATE,CALIBRATION_PATHOVER);return;}//�ж��Ƿ��յ�
	
	//�滮ƽ���ٶ�
	V_parallel=moving_line_velocity_planning_trapezoid(b,s);
	

	
	//������Ŀǰ��λ�ú��յ������
	//���б�ʴ���
	if(ROBOT_NOW.X!=PATH_NOW.X1)
  {
		k3=(ROBOT_NOW.Y-PATH_NOW.Y1)/(ROBOT_NOW.X-PATH_NOW.X1);
		direct_unit_vector_x=sqrt(1.0f/(k3*k3+1.0f));
		direct_unit_vector_y=direct_unit_vector_x*k3;
		
		V_direct=sqrt((error*V_parallel/b)*(error*V_parallel/b)+V_parallel*V_parallel);
		
	}
	else //б�ʲ�����
	{
		direct_unit_vector_x=0;
		direct_unit_vector_y=1.0f;
		
		V_direct=sqrt((error*V_parallel/b)*(error*V_parallel/b)+V_parallel*V_parallel);
	}
	
	//pid���ع������ݻ�ȡ(��ͨ��pid���ز�������С,�����ɵ�λ�������궨)
	pid_position_calculate_error(&line_pid,float_abs(error));
	pid_control=line_pid.output;
	
	//����V_direct����(�ٶȿط���)(ͨ�����յ�Ƚϱ궨)
	if(ROBOT_NOW.Y>PATH_NOW.Y1)direct_unit_vector_y=-direct_unit_vector_y;//һ�����ޱ긺
	if(ROBOT_NOW.X>PATH_NOW.X1)direct_unit_vector_x=-direct_unit_vector_x;//һ�����ޱ긺
	
	
	//����Ŀ���ٶ�
	ROBOT_NOW.target_Vx=pid_control*vertical_unit_vector_x+V_direct*direct_unit_vector_x;
	ROBOT_NOW.target_Vy=pid_control*vertical_unit_vector_y+V_direct*direct_unit_vector_y;
	
}

//��·�����ٶȹ滮--���μӼ���
float moving_line_velocity_planning_trapezoid(float b,float s)
{
	float set_v=0.0f;
	float traveled_s;
	
	traveled_s=s-b;//����ƽ�з����߹���·��
	
	//����������ʻģʽ
	if(moving.V_low_mode==1)
  {
		return set_v=moving.V_low;
	}
	
	if(s<moving.s_slow_down+moving.s_speed_up)
  {
		printf("s is shorter than s_slow_down+moving.s_speed_up!!!\r\n");
    return set_v=moving.V_low;
	};
	
	if(traveled_s<moving.s_speed_up)//���ٽ׶�(�ϸ�����ٶ�����,��traveled_s=moving.s_speed_upʱ�ﵽ����ٶ�)
		set_v=sqrt(moving.V_max*moving.V_max-2.0f*moving.a1*(moving.s_speed_up-traveled_s));//v0*v0=vt*vt-2as(��vt����Ϊv0��Ϊ���,�������ٶȵĿ���)
	else if(traveled_s>moving.s_speed_up&&traveled_s<(s-moving.s_slow_down))//���ٽ׶�
	  set_v=moving.V_max;
	else if(b<moving.s_slow_down)//���ٽ׶�
		set_v=sqrt(moving.V_max*moving.V_max-2.0f*moving.a2*(moving.s_slow_down-b));
	else
	{
		printf("moving_line_velocity_planning_trapezoid is unnormal!!!\r\n");
	  return set_v=moving.V_low;
	}
	return set_v;
}

//ȡfloat���͵ľ���ֵ
float float_abs(float value)
{
	if(value<0)
  {
		value=-value;
	}
	return value;
}



//�ƶ�orת��(�漰�����жϴ���)
void go_straight_or_turn_around(void)
{
if(ROBOT_TURNAROUND==TURNAROUND_STAYBACK)
{
	static u8 prevent_repeat=0;//��ֹ�ظ��������ת�����
	if(prevent_repeat==0)
  { 
	
		if(!(((feedback.trace_infrared&0x1f)>>3)<0x03&&(feedback.trace_infrared&0x03)<0x03))//ֻҪ�������߶�ѹ��
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
		else //���߶�ѹ��
		{
       ROBOT_NOW.target_Vy=0;
       turnaround_interrupt(ROBOT_STATE);
			 prevent_repeat=1;
		}
	}
	if(prevent_repeat==1&&BEFORE_STATE==TURNAROUND_TO_PUT)//����У׼
	{
		calibration_interrupt(ROBOT_STATE,CALIBRATION_WALL);
    prevent_repeat=2;
		return;
	}
	else if(prevent_repeat==1&&BEFORE_STATE!=TURNAROUND_TO_PUT)//�������Ҫ����У׼
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




//ת��������ʱ��180��
void whirling_180_angle(void)//����ʱ��Ϊ��
{ 
	static u8 whirling_count=0;
	if(ROBOT_TURNAROUND==TURNAROUND_START|ROBOT_TURNAROUND==TURNAROUND_IT|ROBOT_TURNAROUND==TURNAROUND_STAYBACK)
	{		
		ROBOT_NOW.target_Vx=ROBOT_NOW.target_Vy=0.0f;
		pid_position_calculate(&whirling_pid,ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw,180);
	  ROBOT_NOW.target_w=whirling_pid.output;
		if(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw<1)whirling_count++;
		else whirling_count--;
		if(whirling_count>5)//���ݱȽ��ȶ����־���(���1�����µ�ʱ�����1�����ϵ�ʱ��һ���̶Ⱦͱ�־���)
    {
			ROBOT_TURNAROUND=TURNAROUND_SUCCEEDED;
			whirling_count=0;
		}
		
	}
}


