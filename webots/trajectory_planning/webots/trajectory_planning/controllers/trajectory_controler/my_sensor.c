#include  "trajectory.h"
//gps
WbDeviceTag gps;
const double *value=NULL;
double sensor_v;
//pen
WbDeviceTag pen;
int gps_point_count=1;

void sensor_init(void)
{
  gps_init();
  pen_init();
}
//gps
void gps_init(void)
{
  gps=wb_robot_get_device("robot_gps");
  wb_gps_enable(gps,TIME_STEP);
}


void gps_get_data(void)
{
  value=wb_gps_get_values(gps);
  printf("x:%f y:%f z:%f -->",value[0],value[1],value[2]);
  sensor_v=wb_gps_get_speed(gps);
  printf("v:%f   ----[%d]\n",sensor_v,gps_point_count++);
  
}
//pen
void pen_init(void)
{
  pen=wb_robot_get_device("my_pen");
  wb_pen_write(pen,1);
  wb_pen_set_ink_color(pen,0x009aff,0.5);
}

