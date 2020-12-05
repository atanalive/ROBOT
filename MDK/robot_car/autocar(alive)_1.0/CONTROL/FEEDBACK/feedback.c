#include "feedback.h"
#include "path.h"
#include "pid.h"
#include "math.h"
/***********������************/
struct FEEDBACK feedback={0,0,0,0,0,0};

PID pathover_pid;
static u8 start_calibration=0;

/***************��������С������Ϣ********************/

//�����õ������˵�������ʵ���ٶ�
//�����ٶ�ת�������������ٶ�(����,����д��v��С��������ľ�����ͨ����ת�任�õ�)(ǰ�ߺ�cpu,�����˹����������󼴿�)
void wheelvelocity_to_coordinateAxis_changeover(void)//coordinate Axis:������
{
	float thetainit=ROBOT_NOW.initial_yaw*pi/180.0f;//���Ǻ����õ��ǻ�����
	float theta=(ROBOT_NOW.yaw*pi/180.0f)-thetainit;
	ROBOT_NOW.Vx=(-cos(theta))*encoder.v[0]+(1.0f/2*cos(theta)+sqrt(3)/2.0f*sin(theta))*encoder.v[1]+(1.0f/2*cos(theta)-sqrt(3)/2.0f*sin(theta))*encoder.v[2];
	ROBOT_NOW.Vy=sin(theta)*encoder.v[0]+(-1.0f/2*sin(theta)+sqrt(3)/2.0f*cos(theta))*encoder.v[1]+(-1.0f/2*sin(theta)-sqrt(3)/2.0f*cos(theta))*encoder.v[2];
	//ROBOT_NOW.w=L*(encoder.v[0]+encoder.v[1]+encoder.v[2]);
}


//���˶�ǽУ׼(У׼�����ǲ���ֵ)
void step_back_wall_calibration(void)
{
	//��ǽ��У׼(��ʼֱ�Ӻ���)//У׼�����ǻ�׼
	 if(ROBOT_CALIBRATION==CALIBRATION_WALL)
	 {
		 ROBOT_NOW.target_Vy=-100;

		 if(feedback.control_calibration==1)
     {
			 feedback.count_time=time.taskcount;
			 feedback.control_calibration=0;
		 }
		 else if((time.taskcount-feedback.count_time)>=1000&&time.taskcount!=0)//����1��(ǰ�治������Ͳ�������������)
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

//����У׼
void black_calibration(void)
{
		//���ݺ���У׼
	if(ROBOT_CALIBRATION==CALIBRATION_BLACK)
	{
		u8 counter_clockwise,clockwise;
	  if((feedback.trace_infrared&0x1f)==0x1f)ROBOT_NOW.target_Vy=80;
	  else
	  {
	   ROBOT_NOW.target_Vy=0;
	   if(((feedback.trace_infrared&0x1f)>>3)<3)clockwise=1;//���ѹ��
	   else if((feedback.trace_infrared&0x03)<3)counter_clockwise=1;//�ұ�ѹ��
	
	   if((feedback.trace_infrared&0x1f)==0x00)
	   {
	   	 feedback.black_calibration=0;
       //��ʼ�����긳ֵ
	   }
	   else if(clockwise&(!counter_clockwise))ROBOT_NOW.w=-10;
	   else if((!clockwise)&counter_clockwise)ROBOT_NOW.w=10;
     }
   }
}

//·���յ�У׼����
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
	else //б�ʲ�����
	{
		direct_unit_vector_x=0;
		direct_unit_vector_y=1.0f;
	}
	
	s=sqrt((PATH_NOW.Y1-PATH_NOW.Y0)*(PATH_NOW.Y1-PATH_NOW.Y0)+(PATH_NOW.X1-PATH_NOW.X0)*(PATH_NOW.X1-PATH_NOW.X0));//���ڵ����굽·���յ�ľ���
	
	//pid�������ݻ�ȡ(��ͨ��pid���ػ�ò�������С,�����ɵ�λ�������궨)
	pid_position_calculate_error(&pathover_pid,s);
	V_direct=pathover_pid.output;
	
	//����V_direct����(�ٶȿط���)(ͨ�����յ�Ƚϱ궨)
	if(ROBOT_NOW.Y>PATH_NOW.Y1)direct_unit_vector_y=-direct_unit_vector_y;//һ�����ޱ긺
	if(ROBOT_NOW.X>PATH_NOW.X1)direct_unit_vector_x=-direct_unit_vector_x;//һ�����ޱ긺
	
	
	//����Ŀ���ٶ�
	ROBOT_NOW.target_Vx=V_direct*direct_unit_vector_x;
	ROBOT_NOW.target_Vy=V_direct*direct_unit_vector_y;//pid����
	
	}
}


	//���ڻ��robot��x��y����������ϵ������
/*���뵽��������Ʒ���
	
	1��У׼��λ��ͨ������������y�ᣬx���ü��⣬��ͨ�����߷�������
	2��x��y�ᶼ�ü��⣬����y��ͻ�䣬����������������̨��͹���������ԱȽϸߡ�(ͬ��ͨ�����߷�������)
	3���ö�ʱ��(1ms)���ٶ������㣬�������y�����꣬��ͨ�����߷�������
	��˼����2�����ȽϿ��У����ö�����
*/


//�����������꣬ͨ��������߷���У׼
//�����ǻ�ýǶȺͽ��ٶ�
void update_robot_state(void)
{
	float yaw;
  mpu_dmp_get_yaw_data(&yaw);
	ROBOT_NOW.yaw=yaw;
	ROBOT_NOW.w=get_yaw_angular_velocity();

	vl53l0x_general_start(dev,0);//��ü���������(x����)mm
	//����y���꣬�����ݴ�
	
	
}



//ת��ģʽ�жϺ���
void turnaround_interrupt(enum ROBOT_STATE_TYPE LAST)
{
	LAST_STATE=LAST;
	ROBOT_STATE=TURNAROUND;
  ROBOT_TURNAROUND=TURNAROUND_IT;
}


//У׼ģʽ�жϺ���
void calibration_interrupt(enum ROBOT_STATE_TYPE LAST,enum CALIBRATION_TYPE TYPE)
{
	LAST_STATE=LAST;
	ROBOT_STATE=CALIBRATION;
	ROBOT_CALIBRATION=TYPE;
}

