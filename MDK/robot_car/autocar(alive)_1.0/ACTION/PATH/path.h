#ifndef __PATH_H
#define __PATH_H
#include "encoder.h"
#include "sys.h"

#define car_xdistance  20  //����С�����ĵ������������Ϊ20
#define car_ydistance  20  //����С�����ĵ�ǰ�����˾���Ϊ20

//·�����ͱ�ǩ
enum pathtype
{
	START_TO_MIDDLE_pathtype,
	MIDDLE_TO_TABLE_pathtype,
	TURNAROUND_TO_PUT_pathtype,
	TURNAROUND_TO_TABLE_pathtype,
	OVER_TO_START_pathtype,
};


//·�����ͣ��ֶ�
typedef struct Path
{
	enum pathtype PATH_STATE;
	float X0;//�������ĵ�����
	float Y0;
  float X1;//Ŀ�ĵص�����
  float Y1;
	float vmax;//����ٶ����ƣ�Ϊ0������
}PATH_type;

extern PATH_type PATH_NOW;//Ŀǰ��״̬
extern PATH_type PATH_START_TO_MIDDLE;//��ʼ���м�
extern PATH_type PATH_MIDDLE_TO_TABLE[3];//�м䵽����̨
extern PATH_type PATH_TURNAROUND_TO_PUT[3][3];//����̨��Ͷ����
extern PATH_type PATH_TURNAROUND_TO_TABLE[3][3];//Ͷ����������̨
extern PATH_type PATH_OVER_TO_START[3];//�����ص���ʼ��

void choice_path(void);


#endif

