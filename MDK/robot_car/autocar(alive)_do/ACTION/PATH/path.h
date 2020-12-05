#ifndef __PATH_H
#define __PATH_H
#include "encoder.h"
#include "sys.h"

#define car_xdistance  20  //假设小车中心到左右两侧距离为20
#define car_ydistance  20  //假设小车中心到前后两端距离为20

//路径类型标签
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

//路径类型，分段
typedef struct Path
{
	enum pathtype now_path;//moving里面帮助判断怎么走的参数
	enum LOCATION_PATH now_location;//同上
	enum LOCATION_PATH next_location;//同上
}PATH_type;



extern PATH_type PATH_NOW;//目前的状态
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

