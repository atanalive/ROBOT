#include "trajectory.h"


WbMouseState mouse;
void interaction_init(void)
{
  wb_keyboard_enable(TIME_STEP);
  wb_mouse_enable(TIME_STEP);
  wb_mouse_enable_3d_position();
}

int choose_mode(void)
{
  printf("请选择模式：0(键盘操控模式) 1(轨迹规划模式)\n");
  while(wb_robot_step(TIME_STEP) != -1)
  {
   switch(wb_keyboard_get_key())
   {
     case '0':
       printf("您选择了键盘操控模式\n");
       return 0;
       break;
     case '1':
       printf("您选择了轨迹规划模式\n");
       return choose_trajectory_type();
       break;
     default:
       break;
   }
  }
  return -1;
}
int choose_trajectory_type(void)
{
  printf("请选择轨迹规划的类型：a(bezier) b(B-splines)\n");
  while(wb_robot_step(TIME_STEP) != -1)
  {
   switch(wb_keyboard_get_key())
   {
     case 'a':
     case 'A':
       printf("您选择了bezier轨迹规划\n");
       t0=t_start=wb_robot_get_time();//得到起始时间
       
       //初始化轨迹规划变量
       curve_times=(point_num-1)/3;//曲线计数
       each_t=target_t/curve_times;//每条曲线的时间长度
       curve_times=0;//取得每段时间后，将当前段数重置为0
       printf("curve:%d each_t:%f",curve_times,each_t);
     
       return 1;
       break;
     case 'b':
     case 'B':
       printf("您选择了B-splines轨迹规划\n");
       t0=t_start=wb_robot_get_time();
       
       curve_times=point_num-1;//曲线计数
       each_t=target_t/curve_times;//每条曲线的时间长度
       curve_times=0;//取得每段时间后，将当前段数重置为0
       printf("curve:%d each_t:%f",curve_times,each_t);
       return 2;
       break;
     default:
       break;
   }
  }
  return -1;
}

void mouse_get_position(void)
{
 mouse=wb_mouse_get_state();
 printf("x:%f y:%f z:%f \n",mouse.x,mouse.y,mouse.z);
}

