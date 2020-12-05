#include "fsm.h"
#include "main.h"

/***************������****************/
enum ROBOT_STATE_TYPE ROBOT_STATE;//��ǰ״̬
enum ROBOT_STATE_TYPE LAST_STATE;//��¼��������һ��״̬������ʵ���жϹ���
enum ROBOT_STATE_TYPE BEFORE_STATE;//����ʵ�ֶ����ж� 
struct ROBOT_NOW_TYPE ROBOT_NOW;//����������ʵʱ״̬
struct state_control STATE_COUNT={0,0,1};//��������״̬
enum CALIBRATION_TYPE ROBOT_CALIBRATION;//У׼״̬
enum ROBOT_VISION_TYPE ROBOT_VISION;//�Ӿ�����
enum ROBOT_GRAB_TYPE ROBOT_GRAB;//ץȡ״̬
enum ROBOT_PUT_TYPE ROBOT_PUT;//Ͷ��
enum ROBOT_TRACKING_TYPE ROBOT_TRACKING;//�ƶ�
enum ROBOT_TURNAROUND_TYPE ROBOT_TURNAROUND;//ת��

//����״̬��
void fsm_init(void)
{
	ROBOT_STATE=CALIBRATION;
	ROBOT_GRAB=GRAB_STOP;
	ROBOT_TRACKING=TRACKING_STOP;
	ROBOT_PUT=PUT_STOP;
	ROBOT_TURNAROUND=TURNAROUND_STOP;
	ROBOT_VISION=VISION_STOP;
	ROBOT_CALIBRATION=CALIBRATION_STOP;
}


//�ο�ʦ�ֵ�״̬�����룬�����˸Ķ�
//ʵ����·���������·�����ٵ��ں�,�����������жϽ���״̬ģʽ(�����ģʽ�µ��ж���̬)
//�ж��Ƿ������һ״̬�������л�״̬
//���߼�˳���г���״̬��������
void FSM_WORK(void)//���л�״̬������
{
	switch(ROBOT_STATE)
  {
		//����ģʽ�����ص���ʼ��ʱ�������ģʽ��
		case STANDBY:
		{
			ROBOT_GRAB=GRAB_STOP;
      ROBOT_TRACKING=TRACKING_STOP;
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_VISION=VISION_STOP;
			ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_TURNAROUND=TURNAROUND_STOP;
		}
		break;
		//У׼ģʽ(�Ա��жϴ���)
		case CALIBRATION:
		{
			if(STATE_COUNT.CALIBRATION_count==1)//��һ��У׼
			{
				ROBOT_GRAB=GRAB_STOP;
        ROBOT_TRACKING=TRACKING_STOP;
	      ROBOT_PUT=PUT_STOP;
	      ROBOT_TURNAROUND=TURNAROUND_STOP;
	      ROBOT_VISION=VISION_STOP;
				
				ROBOT_CALIBRATION=CALIBRATION_WALL;
				if(ROBOT_CALIBRATION==CALIBRATION_SUCCEEDED)
			  {
			    STATE_COUNT.CALIBRATION_count=2;//��Ϊ2��������һ��У׼ģʽ����������У׼����ģʽ
					VISION_SEND_CONTROL=VISION_ASK_JUDGEMENT_MODE;//����ݮ�ɷ���ʶ������
					ROBOT_CALIBRATION=CALIBRATION_STOP;
				  ROBOT_STATE=START_TO_MIDDLE;
			  }
			}
			else if(ROBOT_CALIBRATION==CALIBRATION_WALL||ROBOT_CALIBRATION==CALIBRATION_BLACK)//ͨ�������жϵĲ�������(��ǽ�����У׼)
      {
				ROBOT_GRAB=GRAB_STOP;
        ROBOT_TRACKING=TRACKING_STOP;
	      ROBOT_PUT=PUT_STOP;
	      ROBOT_TURNAROUND=TURNAROUND_STOP;
	      ROBOT_VISION=VISION_STOP;
				if(ROBOT_CALIBRATION==CALIBRATION_SUCCEEDED)
				{
					ROBOT_CALIBRATION=CALIBRATION_STOP;
					ROBOT_STATE=LAST_STATE;//�ص�·����������
				}
			}
			else if(ROBOT_CALIBRATION==CALIBRATION_PATHOVER)//·���յ�У׼�жϴ���
      {
				ROBOT_GRAB=GRAB_STOP;
        ROBOT_TRACKING=TRACKING_STOP;
	      ROBOT_PUT=PUT_STOP;
	      ROBOT_TURNAROUND=TURNAROUND_STOP;  
	      ROBOT_VISION=VISION_STOP;
				if(ROBOT_CALIBRATION==CALIBRATION_SUCCEEDED)
        {
					ROBOT_CALIBRATION=CALIBRATION_STOP;
					ROBOT_STATE=LAST_STATE;
					ROBOT_TRACKING=TRACKING_SUCCEEDED;
				}
			}
		}
		break;
		//ת��ģʽ(�Ա��жϴ���)
		case TURNAROUND:
		{
			if(ROBOT_TURNAROUND!=TURNAROUND_IT)//ֱ�Ӵ���
			{	
			  ROBOT_GRAB=GRAB_STOP;
        ROBOT_TRACKING=TRACKING_STOP;
	      ROBOT_PUT=PUT_STOP;
	      ROBOT_VISION=VISION_STOP;
			  ROBOT_CALIBRATION=CALIBRATION_STOP;
			
			  ROBOT_TURNAROUND=TURNAROUND_STAYBACK;
				if(ROBOT_TURNAROUND==TURNAROUND_SUCCEEDED)//�����жϷ���
        {
					ROBOT_TURNAROUND=TURNAROUND_STOP;
					ROBOT_STATE=BEFORE_STATE;
				}
			}
			else if(ROBOT_TURNAROUND==TURNAROUND_IT)//�жϴ���
      {
			  ROBOT_GRAB=GRAB_STOP;
        ROBOT_TRACKING=TRACKING_STOP;
	      ROBOT_PUT=PUT_STOP;
	      ROBOT_TURNAROUND=TURNAROUND_STOP;
	      ROBOT_VISION=VISION_STOP;
			  ROBOT_CALIBRATION=CALIBRATION_STOP;
				
				if(ROBOT_TURNAROUND==TURNAROUND_SUCCEEDED)
        {
					ROBOT_TURNAROUND=TURNAROUND_STOP;
					ROBOT_STATE=LAST_STATE;
				}
			}
		}
		//��ʼ���м�
		case START_TO_MIDDLE:
		{
		  ROBOT_GRAB=GRAB_STOP;
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_TURNAROUND=TURNAROUND_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			
			ROBOT_VISION=VISION_START;
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEEDED)
			{		
				ROBOT_VISION=VISION_STOP;//ֹͣ�Ӿ����ݻ�ȡ
				calculate_grab_and_put_order();//����ץȡ�Ͱڷ�˳��
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=MIDDLE_TO_TABLE;
			}
		}
		break;
		case MIDDLE_TO_TABLE://�м䵽����̨
		{
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_TURNAROUND=TURNAROUND_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEEDED)
      {
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=GRAB;
			}
		}
		break;
		case GRAB://ץȡ
		{
	    ROBOT_PUT=PUT_STOP;
	    ROBOT_TURNAROUND=TURNAROUND_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_TRACKING=TRACKING_STOP;
			
			ROBOT_GRAB=GRAB_START;
			if(ROBOT_GRAB==GRAB_SUCCEEDED)
      {
				ROBOT_STATE=TURNAROUND;//����ת��
				ROBOT_GRAB=GRAB_STOP;
				BEFORE_STATE=TURNAROUND_TO_PUT;//ת����״̬�л���TURNAROUND_TO_PUT
			}
		}
		break;
		case TURNAROUND_TO_PUT://ת����ȥ��Ͷ����
		{
			ROBOT_PUT=PUT_STOP;
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_TURNAROUND=TURNAROUND_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEEDED)
      {
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=PUT;
				STATE_COUNT.TURNAROUND_TO_PUT_count++;
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==3)//ץȡͶ�Ź������
        {
					ROBOT_TRACKING=TRACKING_STOP;
					ROBOT_STATE=OVER_TO_START;
					
				}
			}
	  }
		break;
		case PUT://����
		{
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_TURNAROUND=TURNAROUND_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_TRACKING=TRACKING_STOP;
			
			ROBOT_PUT=PUT_START;			
			if(ROBOT_PUT==PUT_SUCCEEDED)
      {
				ROBOT_PUT=PUT_STOP;
				ROBOT_STATE=TURNAROUND;//ת��
				BEFORE_STATE=TURNAROUND_TO_TABLE;//ת������֮���л�TURNAROUND_TO_TABLE
				
			}
		}
		break;
		case TURNAROUND_TO_TABLE://ת����ص�����̨
		{
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_TURNAROUND=TURNAROUND_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_PUT=PUT_STOP;	
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEEDED)
      {
				ROBOT_TRACKING=TRACKING_STOP;
				STATE_COUNT.TURNAROUND_TO_TABLE_count++;
				ROBOT_STATE=GRAB;
			}
		}
		break;
		case OVER_TO_START://�����ص���ʼ��
    {
			ROBOT_GRAB=GRAB_STOP;
	    ROBOT_TURNAROUND=TURNAROUND_STOP;
	    ROBOT_VISION=VISION_STOP;
	    ROBOT_CALIBRATION=CALIBRATION_STOP;
			ROBOT_PUT=PUT_STOP;	
			
			ROBOT_TRACKING=TRACKING_START;
			if(ROBOT_TRACKING==TRACKING_SUCCEEDED)
      {
				ROBOT_TRACKING=TRACKING_STOP;
				ROBOT_STATE=STANDBY;
			}
		}
		break;
	}
}




