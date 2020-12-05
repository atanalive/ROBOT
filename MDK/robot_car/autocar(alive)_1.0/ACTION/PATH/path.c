#include "path.h"
#include "fsm.h"
#include "usart.h"
#include "feedback.h"

/***************·���Ķ���****************/
//��һ������������ʶ��ģ���ʱû�õ��������Ժ���չ�ܿ��ܻ��õ�����Ϊ��װ����PATH_NOW��

//����·��
//ÿ�׶���1000,��ȷ������.�����½ǵ�ֱ��Ϊ����ϵ,ͨ���������͵����߷�����������

//Ŀǰ��״̬
PATH_type PATH_NOW={START_TO_MIDDLE_pathtype,0};


//��ʼ���м�
PATH_type PATH_START_TO_MIDDLE={START_TO_MIDDLE_pathtype,0,0,1200,600,0};

//�м䵽����̨
PATH_type PATH_MIDDLE_TO_TABLE[3]=
{
	{MIDDLE_TO_TABLE_pathtype,1200,600,1050,160+car_ydistance,0},
	{MIDDLE_TO_TABLE_pathtype,1200,600,1050+150,160+car_ydistance,0},
	{MIDDLE_TO_TABLE_pathtype,1200,600,1050+300,160+car_ydistance,0},
};

//����̨��Ͷ����
PATH_type PATH_TURNAROUND_TO_PUT[3][3]=//��һ��������ʾ����̨������123,�ڶ���������ʾͶ����������123.��������
{
	{
	 {TURNAROUND_TO_PUT_pathtype,1050,160+car_ydistance,1050,900-car_ydistance,0},
	 {TURNAROUND_TO_PUT_pathtype,1050,160+car_ydistance,1050+150,900-car_ydistance,0},
	 {TURNAROUND_TO_PUT_pathtype,1050,160+car_ydistance,1050+300,900-car_ydistance,0},
	},
	{
	 {TURNAROUND_TO_PUT_pathtype,1200,160+car_ydistance,1050,900-car_ydistance,0},
	 {TURNAROUND_TO_PUT_pathtype,1200,160+car_ydistance,1050+150,900-car_ydistance,0},
	 {TURNAROUND_TO_PUT_pathtype,1200,160+car_ydistance,1050+300,900-car_ydistance,0},
	},
	{
	 {TURNAROUND_TO_PUT_pathtype,1350,160+car_ydistance,1050,900-car_ydistance,0},
	 {TURNAROUND_TO_PUT_pathtype,1350,160+car_ydistance,1050+150,900-car_ydistance,0},
	 {TURNAROUND_TO_PUT_pathtype,1350,160+car_ydistance,1050+300,900-car_ydistance,0},
	},
};

//Ͷ����������̨
PATH_type PATH_TURNAROUND_TO_TABLE[3][3]=
{
  {
	 {TURNAROUND_TO_TABLE_pathtype,1050,900-car_ydistance,1050,160+car_ydistance,0},
	 {TURNAROUND_TO_TABLE_pathtype,1050,900-car_ydistance,1050+150,160+car_ydistance,0},
	 {TURNAROUND_TO_TABLE_pathtype,1050,900-car_ydistance,1050+300,160+car_ydistance,0},
	},
	{
	 {TURNAROUND_TO_TABLE_pathtype,1200,900-car_ydistance,1050,160+car_ydistance,0},
	 {TURNAROUND_TO_TABLE_pathtype,1200,900-car_ydistance,1050+150,160+car_ydistance,0},
	 {TURNAROUND_TO_TABLE_pathtype,1200,900-car_ydistance,1050+300,160+car_ydistance,0},
	},
	{
	 {TURNAROUND_TO_TABLE_pathtype,1350,900-car_ydistance,1050,160+car_ydistance,0},
	 {TURNAROUND_TO_TABLE_pathtype,1350,900-car_ydistance,1050+150,160+car_ydistance,0},
	 {TURNAROUND_TO_TABLE_pathtype,1350,900-car_ydistance,1050+300,160+car_ydistance,0},
	},
};

//�����ص���ʼ��
PATH_type PATH_OVER_TO_START[3]=
{
	{OVER_TO_START_pathtype,1050,900-car_ydistance,150,150,0},
	{OVER_TO_START_pathtype,1200,900-car_ydistance,150,150,0},
	{OVER_TO_START_pathtype,1350,900-car_ydistance,150,150,0},
};


/******************·����ѡ��*****************/
//ѡ��·��

void choice_path(void)
{
	if(ROBOT_TRACKING==TRACKING_START)//����·��ѡ���Ƿ�����
{
	//��ʼ���м�
  if(ROBOT_STATE==START_TO_MIDDLE)
  {
		PATH_NOW=PATH_START_TO_MIDDLE;
		return;
	}
	//�м䵽����̨
	if(ROBOT_STATE==MIDDLE_TO_TABLE)
  {
		switch(ROBOT_NOW.grab_order)
		{
			case 1:
			case 2:
				PATH_NOW=PATH_MIDDLE_TO_TABLE[0];
			break;
			case 3:
			case 4:
				PATH_NOW=PATH_MIDDLE_TO_TABLE[1];
			break;
			case 5:
			case 6:
				PATH_NOW=PATH_MIDDLE_TO_TABLE[2];			
      break;			
		}
		return;
	}
	//����̨��Ͷ����
	if(ROBOT_STATE==TURNAROUND_TO_PUT)
  {
		u8 a=0;
		u8 b=0;
		switch((u16)PATH_NOW.X1)
		{
			case 1050:
				    a=1;
			break;
			case 1200:
				    a=2;
			break;
			case 1350:
						a=3;	
      break;
      default:
        printf("PATH:TURNAROUND_TO_PUT was wrong");				
		}
		switch(ROBOT_NOW.put_order)
		{
			case 1:
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==2){b=3;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==1){b=2;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==0){b=1;break;}
				printf("PATH:TURNAROUND_TO_PUT was wrong");
				break;
			case 2:
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==2){b=2;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==1){b=3;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==0){b=1;break;}
				printf("PATH:TURNAROUND_TO_PUT was wrong");
			  break;
			case 3:
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==2){b=3;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==1){b=1;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==0){b=2;break;}
				printf("PATH:TURNAROUND_TO_PUT was wrong");
				break;
			case 4:
        if(STATE_COUNT.TURNAROUND_TO_PUT_count==2){b=1;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==1){b=3;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==0){b=2;break;}
				printf("PATH:TURNAROUND_TO_PUT was wrong");
			  break;
			case 5:
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==2){b=2;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==1){b=1;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==0){b=3;break;}
				printf("PATH:TURNAROUND_TO_PUT was wrong");
				break;
			case 6:
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==2){b=1;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==1){b=2;break;}
				if(STATE_COUNT.TURNAROUND_TO_PUT_count==0){b=3;break;}
        printf("PATH:TURNAROUND_TO_PUT was wrong");			
        break;			
		}
		PATH_NOW=PATH_TURNAROUND_TO_PUT[a-1][b-1];
		return;
	}
	//Ͷ����������̨
	if(ROBOT_STATE==TURNAROUND_TO_TABLE)
  {
		u8 a=0;
		u8 b=0;
		switch((u16)PATH_NOW.X1)
		{
			case 1050:
				    a=1;
			break;
			case 1200:
				    a=2;
			break;
			case 1350:
						a=3;	
      break;
      default:
        printf("PATH:TURNAROUND_TO_TABLE was wrong");				
		}
		switch(ROBOT_NOW.grab_order)
		{
			case 1:
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==1){b=3;break;}
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==0){b=2;break;}
				printf("PATH:TURNAROUND_TO_TABLE was wrong");
				break;
			case 2:
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==1){b=2;break;}
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==0){b=3;break;}
				printf("PATH:TURNAROUND_TO_TABLE was wrong");
			  break;
			case 3:
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==1){b=3;break;}
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==0){b=1;break;}
				printf("PATH:TURNAROUND_TO_TABLE was wrong");
				break;
			case 4:
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==1){b=1;break;}
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==0){b=3;break;}
				printf("PATH:TURNAROUND_TO_TABLE was wrong");
			  break;
			case 5:
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==1){b=2;break;}
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==0){b=1;break;}
				printf("PATH:TURNAROUND_TO_TABLE was wrong");
				break;
			case 6:
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==1){b=1;break;}
				if(STATE_COUNT.TURNAROUND_TO_TABLE_count==0){b=2;break;}
        printf("PATH:TURNAROUND_TO_TABLE was wrong");			
        break;			
		}
		PATH_NOW=PATH_TURNAROUND_TO_TABLE[a-1][b-1];
		return;
	}
	//�����ص���ʼ��
	if(ROBOT_STATE==OVER_TO_START)
  {
		switch(ROBOT_NOW.put_order)
		{
			case 4:
			case 6:
				PATH_NOW=PATH_OVER_TO_START[0];
			break;
			case 2:
			case 5:
				PATH_NOW=PATH_OVER_TO_START[1];
			break;
			case 1:
			case 3:
				PATH_NOW=PATH_OVER_TO_START[2];			
      break;			
		}
		return;
	}
	if(ROBOT_STATE==CALIBRATION)
  {
		//
		return;
	}
}	
}

//�յ�λ�ý�������
void end_point_correct(void)
{
	
}



