#include "trajectory.h"
//定义电机
WbDeviceTag motor[4];
const char *motor_name[]=
{
  "motor_1",
  "motor_2",
  "motor_3",
  "motor_4"
};

//定义舵机
WbDeviceTag steer[4];
const char *steer_name[]=
{
  "steer_1",
  "steer_2",
  "steer_3",
  "steer_4"
};

//定义变量
double velocity=0;//速度（两种模式共用）
//键盘移动模式特有变量
float keyboard_angle=0;//键盘移动模式角度
int get_data_count=1;//运行频率记录量

//轨迹规划变量
double angle=0;
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
int curve_times;//曲线计数
double target_t=10;//目标时间（单位：秒）
double each_t;//每条曲线的时间长度
double t_start;//轨迹规划起始时间
double t0;//每段小轨迹起始时间

void moving_init(void)
{
  //初始化电机
  for(int i=0;i<4;i++)
  {
    motor[i]=wb_robot_get_device(motor_name[i]);
    wb_motor_set_control_pid(motor[i],2.0,0.5,0.2);
    wb_motor_set_velocity(motor[i],0);
    wb_motor_set_position(motor[i],INFINITY);
  }
  
  //初始化舵机
  for(int i=0;i<4;i++)
  {
    steer[i]=wb_robot_get_device(steer_name[i]);
  }
}
//包含两种行走方式
void keyboard_moving(void)
{
  switch (wb_keyboard_get_key()) {
      case WB_KEYBOARD_UP:
        velocity=10;
        keyboard_angle=0;
        break;
      case WB_KEYBOARD_DOWN:
        velocity=-10;
        keyboard_angle=0;
        break;
      case WB_KEYBOARD_LEFT:
        velocity=10;
        keyboard_angle=-π/2;
        break;
      case WB_KEYBOARD_RIGHT:
        velocity=10;
        keyboard_angle=π/2;
        break;
      case ' ':
        keyboard_angle=0;
        velocity=0;
        break;
      case WB_KEYBOARD_CONTROL|WB_KEYBOARD_UP:
        velocity=5;
        break;
      case WB_KEYBOARD_CONTROL|WB_KEYBOARD_DOWN:
        velocity=-5;
        break;
      case WB_KEYBOARD_CONTROL|WB_KEYBOARD_LEFT:
        keyboard_angle-=π/36;
        break;
      case WB_KEYBOARD_CONTROL|WB_KEYBOARD_RIGHT:
        keyboard_angle+=π/36;
        break;
      case WB_KEYBOARD_CONTROL|'.':
        velocity=0;
        break;
      case WB_KEYBOARD_CONTROL|'+':
        wb_robot_step(10);
        gps_get_data();
        break;
      case WB_KEYBOARD_CONTROL|'-':
        wb_robot_step(10);
        mouse_get_position();
        break;
        
      default:
        break;
   }
   for(int i=0;i<4;i++)
   {
     wb_motor_set_position(steer[i],keyboard_angle);
     wb_motor_set_velocity(motor[i],velocity);
   }
   get_data_count++;
   if(get_data_count%10==0)
   {
     gps_get_data();
   }
}

void trajectory_moving(int type)
{
  if(type==1)//bezier
  {
    bezier_3_calculate();
  }
  else if(type==2)//B-splines
  {
    B_splines_3_calculate();
  }
 for(int i=0;i<4;i++)
  {
    wb_motor_set_position(steer[i],angle);//角度
    wb_motor_set_velocity(motor[i],v_to_w*velocity);//角速度
  }
}

void bezier_3_calculate(void)
{
   double x;//归一化变量
   double t_now;//记录当前时刻模拟时间
   t_now=wb_robot_get_time();
  
   if(t_now-t_start>=target_t)
   {
     velocity=0;
     return;
   }
   //printf("each_t is:%f \n",each_t);
   //printf("t0 is :%f \n",t0);
   //printf("t_now is:%f \n",t_now);
   x=(t_now-t0)/each_t;
   //printf("x is:%f \n",x);
   
   if(x>=1.0)
   {
     t0+=each_t;//更新线段起始点时间
     curve_times++;//更新曲线段数
     x=(t_now-t0)/each_t;
   }
   
   ///bezier公式
   /*
   //x坐标
   robot.target_position[0]=trajectory_point[0][curve_times*3+0]*pow((1-x),3)+
                            3*trajectory_point[0][curve_times*3+1]*x*pow((1-x),2)+
                            3*trajectory_point[0][curve_times*3+2]*pow(x,2)*(1-x)+
                            trajectory_point[0][curve_times*3+3]*pow(x,3);
   //z坐标
   robot.target_position[1]=trajectory_point[1][curve_times*3+0]*pow((1-x),3)+
                            3*trajectory_point[1][curve_times*3+1]*x*pow((1-x),2)+
                            3*trajectory_point[1][curve_times*3+2]*pow(x,2)*(1-x)+
                            trajectory_point[1][curve_times*3+3]*pow(x,3);
   */
   
   //x速度
   robot.target_v[0]=((-3)*trajectory_point[0][curve_times*3+0]*pow((1-x),2)+
                     3*trajectory_point[0][curve_times*3+1]*(3*x-1)*(x-1)+
                     3*trajectory_point[0][curve_times*3+2]*x*(2-3*x)+
                     3*trajectory_point[0][curve_times*3+3]*pow(x,2))/each_t;
   //z速度                     
   robot.target_v[1]=((-3)*trajectory_point[1][curve_times*3+0]*pow((1-x),2)+
                     3*trajectory_point[1][curve_times*3+1]*(3*x-1)*(x-1)+
                     3*trajectory_point[1][curve_times*3+2]*x*(2-3*x)+
                     3*trajectory_point[1][curve_times*3+3]*pow(x,2))/each_t;
   
   printf("x:%f z:%f v_x:%f v_z:%f \n",robot.target_position[0],robot.target_position[1],robot.target_v[0],robot.target_v[1]);               
   //计算目标速度和角度                  
   calculate_angle_and_velocity(robot.target_v[0],robot.target_v[1]);
}

void B_splines_3_calculate(void)
{
///归一化时间
  double x;//归一化变量
  double t_now;//记录当前时刻模拟时间
  t_now=wb_robot_get_time();

  if(t_now-t_start>=target_t)
  {
    velocity=0;
    return;
  }/*
  printf("each_t is:%f \n",each_t);
  printf("t0 is :%f \n",t0);
  printf("t_now is:%f \n",t_now);*/
  x=(t_now-t0)/each_t;
  //printf("x is:%f \n",x);
 
  if(x>=1.0)
  {
    t0+=each_t;//更新线段起始点时间
    curve_times++;//更新曲线段数
    x=(t_now-t0)/each_t;
  }
  /*
  printf("one :%f  tow :%f  three :%f  four :%f \n",
  trajectory_point[0][curve_times*3+0],
  trajectory_point[0][curve_times*3+1],
  trajectory_point[0][curve_times*3+2],
  trajectory_point[0][curve_times*3+3]);*/
  
  //B-splines公式
  /*
  //方法一
  for(int i=0;i<2;i++)
  {
  double w0=(1/(double)6)*trajectory_point[i][curve_times*3+0]+4*trajectory_point[i][curve_times*3+1]+trajectory_point[i][curve_times*3+2];
  double w1=(-1/(double)2)*(trajectory_point[i][curve_times*3+0]-trajectory_point[i][curve_times*3+2]);
  double w2=(1/(double)2)*(trajectory_point[i][curve_times*3+0]-2*trajectory_point[i][curve_times*3+1]+trajectory_point[i][curve_times*3+2]);
  double w3=(-1/(double)6)*(trajectory_point[i][curve_times*3+0]-3*trajectory_point[i][curve_times*3+1]+3*trajectory_point[i][curve_times*3+2]-trajectory_point[i][curve_times*3+3]);
  robot.target_v[i]=w1+2*w2*x+3*w3*pow(x,2);
  }
  */
  //i=0 x速度    i=1 z速度
  int w0=curve_times+0;
  int w1=curve_times+1;
  int w2=curve_times+2;
  int w3=curve_times+3;
  //处理最后两个点
  if(w2>point_num-1)w2=point_num-1;
  if(w3>point_num-1)w3=point_num-1;
  
  for(int i=0;i<2;i++)
  {
    robot.target_v[i]=((-1/(double)2)*pow((1-x),2)*trajectory_point[i][w0]+
                    (1/(double)2)*(3*pow(x,2)-4*x)*trajectory_point[i][w1]+
                    (1/(double)2)*((-3)*pow(x,2)+2*x+1)*trajectory_point[i][w2]+
                    (1/(double)2)*pow(x,2)*trajectory_point[i][w3])/each_t;
  }
  printf("x:%f z:%f v_x:%f v_z:%f \n",robot.target_position[0],robot.target_position[1],robot.target_v[0],robot.target_v[1]);    
             
  //计算目标速度和角度                  
  calculate_angle_and_velocity(robot.target_v[0],robot.target_v[1]);
}

void calculate_angle_and_velocity(float x_v,float z_v)
{
  velocity=sqrt(pow(x_v,2)+pow(z_v,2));
  angle=atan2(z_v,x_v)-π/2;//atan2(y,x) 返回弧度制 角度顺时针为正
  //防止大角度转轮
  if(angle>π)angle-=2*π;
  else if(angle<-π)angle+=2*π;
  //if(-0.05<x_v&&x_v<0.05)angle=0;
  //printf("x:%f z:%f\n",x_v,z_v);
  printf("velocity:%f angle:%f\n",velocity,angle);
}

