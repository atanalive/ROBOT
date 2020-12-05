#include "trajectory.h"
int mode=-1;
struct ROBOT robot;//机器人结构体

int main(int argc, char **argv) {
  wb_robot_init();
  
  moving_init();
  sensor_init();
  interaction_init();
  
  mode=choose_mode();
  
  while (wb_robot_step(TIME_STEP) != -1) {
   if(mode==1||mode==2)//轨迹规划模式
   {
     trajectory_moving(mode);
   }else if(mode==0)//键盘移动模式
   {
     keyboard_moving();
   }
   
  }
  
  wb_robot_cleanup();
  return 0;
}
