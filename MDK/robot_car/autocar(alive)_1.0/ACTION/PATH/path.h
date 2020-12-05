#ifndef __PATH_H
#define __PATH_H
#include "encoder.h"
#include "sys.h"

#define car_xdistance  20  //假设小车中心到左右两侧距离为20
#define car_ydistance  20  //假设小车中心到前后两端距离为20

//路径类型标签
enum pathtype
{
	START_TO_MIDDLE_pathtype,
	MIDDLE_TO_TABLE_pathtype,
	TURNAROUND_TO_PUT_pathtype,
	TURNAROUND_TO_TABLE_pathtype,
	OVER_TO_START_pathtype,
};


//路径类型，分段
typedef struct Path
{
	enum pathtype PATH_STATE;
	float X0;//车子中心的坐标
	float Y0;
  float X1;//目的地点坐标
  float Y1;
	float vmax;//最大速度限制，为0则不限制
}PATH_type;

extern PATH_type PATH_NOW;//目前的状态
extern PATH_type PATH_START_TO_MIDDLE;//开始到中间
extern PATH_type PATH_MIDDLE_TO_TABLE[3];//中间到物料台
extern PATH_type PATH_TURNAROUND_TO_PUT[3][3];//物料台到投放区
extern PATH_type PATH_TURNAROUND_TO_TABLE[3][3];//投放区到物料台
extern PATH_type PATH_OVER_TO_START[3];//结束回到起始区

void choice_path(void);


#endif

