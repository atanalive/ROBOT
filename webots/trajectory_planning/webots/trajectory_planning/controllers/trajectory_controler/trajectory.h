#ifndef TRAJECTORY_H
#define TRAJECTORY_H

//系统头文件
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/gps.h>
#include <webots/pen.h>
#include <stdio.h>
#include <math.h>
#include <webots/keyboard.h>
#include <webots/mouse.h>

//自定义头文件
#include "moving.h"
#include "my_sensor.h"
#include "interaction.h"

//定义常量
#define TIME_STEP 64
#define π 3.1415926535

extern int mode;

struct ROBOT
{
  float target_position[2];//索引: 0为x 1为z
  float target_v[2];//索引: 0为x 1为z
  float target_a[2];//索引: 0为x 1为z
};
extern struct ROBOT robot;


#endif

