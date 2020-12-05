#ifndef __PATH_H
#define __PATH_H
#include "encoder.h"
#include "sys.h"

#define car_xdistance  20  //����С�����ĵ������������Ϊ20
#define car_ydistance  20  //����С�����ĵ�ǰ�����˾���Ϊ20

//·�����ͱ�ǩ
enum pathtype
{
	stanby,
	calibration,
	start_to_middle,
	middle_to_grab,
	grab_to_put,
	return_to_grab,
	return_to_start,
};

enum LOCATION_PATH
{
	path_left,
	path_right,
	path_middle,
};

//·�����ͣ��ֶ�
typedef struct Path
{
	enum pathtype now_path;//moving��������ж���ô�ߵĲ���
	enum LOCATION_PATH now_location;//ͬ��
	enum LOCATION_PATH next_location;//ͬ��
}PATH_type;



extern PATH_type PATH_NOW;//Ŀǰ��״̬
/*
extern PATH_type PATH_CALIBRATION;
extern PATH_type PATH_START_TO_MIDDLE;
extern PATH_type PATH_MIDDLE_TO_GRAB;
extern PATH_type PATH_GRAB_TO_PUT;
extern PATH_type PATH_RETURN_TO_GRAB;
extern PATH_type PATH_RETURN_TO_START;
*/
void choice_path(void);


#endif

