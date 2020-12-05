#ifndef GROUND_DISPLAY
#define GROUND_DISPLAY

#include <webots/robot.h>
#include <webots/display.h>
#include <webots/supervisor.h>
#include <stdio.h>

#define TIME_STEP 64
#define point_num 31

void draw_path(void);
void draw_robot_moving(void);

extern double trajectory_point[2][point_num];
extern WbDeviceTag ground_display;//画板
extern int h,w;//画板宽高
extern WbNodeRef robot;//机器人节点
extern const double *position;//机器人位置
extern double last_position[3];//机器人上一次位置：用于画线

#endif
