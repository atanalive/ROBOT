#ifndef __MOVING_H
#define __MOVING_H
#include "sys.h"
#include "path.h"


u8 right_double_online_track(u8 v1,u8 v2);
u8 behind_online_track(u8 v1,u8 v2);
u8 right_count_black_track(u8 count);
u8 behind_count_black_track(u8 count);
u8 task_delay_ms(u32 delay_time);
//开始双线到中间 v1高速速度 v2低速速度
void start_to_middle_moving(u8 v1,u8 v2,u8 count);
//开始路段封装函数 
void middle_to_grab_moving(u8 vx,u8 vy,u16 delay1,u16 delay2,enum LOCATION_PATH target);
//抓取路段规划封装函数 count为黑线条数的大小 v1为高速大小 v2为低速大小 
void grap_to_put_moving(u8 count,u8 v1,u8 v2,u16 delay1,enum LOCATION_PATH target);
//回到物料台抓取 v1为水平移动高速速度 v2水平移动低速速度 count操作要压哪根线 v3水平速度 v4竖直速度 delay3水平延时 delay4竖直延时
void return_to_grab_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u16 delay3,u16 delay4,enum LOCATION_PATH target);
//回到起始区 v1为水平移动高速速度 v2水平移动低速速度 count操作要压哪根线 v3水平速度 v4竖直速度 delay3水平延时 delay4竖直延时
void return_to_start_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u16 delay3,u16 delay4,enum LOCATION_PATH target);



void coordinates_to_wheelvelocity_changeover(void);

#endif



