#include "ground_display.h"
//点的大小
#define rectangle_w 10
#define rectangle_h 10

WbDeviceTag ground_display;//画板
int h,w;//画板宽高
WbNodeRef robot;//机器人节点
const double *position;//机器人位置
double last_position[3]={0,0,0};//机器人上一次位置：用于画线


double trajectory_point[2][point_num]={//前者x 后者z
{
-0.000124,-0.000076,0.000087,0.000249,
0.000411,-0.004660,-0.039051,-0.121586,
-0.218213,-0.326499,-0.443009,-0.566656,
-0.693736,-0.817411,-0.937402,-1.059401,
-1.184675,-1.300267,-1.407365,-1.514290,
-1.629354,-1.748922,-1.861642,-1.940337,
-1.976004,-1.969494,-1.971771,-1.982797,
-2.000916,-2.022836,-2.044754
},
{
0.087222,0.125598,0.253621,0.381621,
0.509621,0.637460,0.761466,0.857183,
0.939461,1.006370,1.055552,1.079222,
1.078337,1.043555,0.998956,0.964990,
0.978748,1.037747,1.107869,1.177826,
1.231217,1.273424,1.338090,1.438841,
1.559781,1.685913,1.813137,1.940647,
2.067538,2.193646,2.319756
}
};

int main(int argc, char **argv) {
  /* necessary to initialize webots stuff */
  wb_robot_init();
  draw_path();

  while (wb_robot_step(TIME_STEP) != -1) {
    draw_robot_moving();
  };
  wb_robot_cleanup();

  return 0;
}


void draw_path(void)
{
  ground_display=wb_robot_get_device("ground_display");
  h=wb_display_get_height(ground_display);
  w=wb_display_get_width(ground_display);
  //如果不填充地板的话，会默认被填充黑色
  wb_display_set_color(ground_display,0xffffff);
  wb_display_fill_rectangle(ground_display,0,0,w,h);
  wb_display_set_color(ground_display,(100<<16)+(50<<8)+0);
  
  for(int i=0;i<point_num-1;i++)
  {
    
    //画点
    wb_display_fill_rectangle(ground_display,
    (int)(trajectory_point[0][i]*1000+2500-rectangle_w/2),
    (int)(trajectory_point[1][i]*1000+500-rectangle_h/2),
    rectangle_w,rectangle_h);
    wb_display_fill_oval(ground_display,
    (int)(trajectory_point[0][30]*1000+2500),
    (int)(trajectory_point[1][30]*1000+500),
    rectangle_w/2,rectangle_h/2);
    //画线
    wb_display_draw_line(ground_display,
    (int)(trajectory_point[0][i]*1000+2500),
    (int)(trajectory_point[1][i]*1000+500),
    (int)(trajectory_point[0][i+1]*1000+2500),
    (int)(trajectory_point[1][i+1]*1000+500));
  }
  
}

void draw_robot_moving(void)
{
  robot=wb_supervisor_node_get_from_def("AUTOCAR");
  //得到机器人坐标
  //方法一
  //WbFieldRef transilation_field=wb_supervisor_node_get_field(robot,"translation");
  //const double *transilation=wb_supervisor_field_get_sf_vec3f(transilation_field);
  //方法二
  position=wb_supervisor_node_get_position(robot);
 
  
  //画点
  wb_display_set_color(ground_display,(0<<16)+(170<<8)+255);
  
  wb_display_fill_rectangle(ground_display,
    (int)(position[0]*1000+2500-rectangle_w/2),
    (int)(position[2]*1000+500-rectangle_h/2),
    rectangle_w,rectangle_h);
  wb_display_draw_line(ground_display,
    (int)(last_position[0]*1000+2500),
    (int)(last_position[2]*1000+500),
    (int)(position[0]*1000+2500),
    (int)(position[2]*1000+500)
    );
    for(int i=0;i<3;i++)last_position[i]=position[i];
  //printf("tran :%f\n%f\n%f\n",transilation[0],transilation[1],transilation[2]);
  //printf("posi :%f\n%f\n%f\n",position[0],position[1],position[2]);
}
