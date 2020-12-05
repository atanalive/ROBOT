#ifndef MOVING_H
#define MOVING_H

#define v_to_w 25
#define point_num 31

void moving_init(void);
void trajectory_moving(int type);
void keyboard_moving(void);
void bezier_3_calculate(void);
void B_splines_3_calculate(void);
void calculate_angle_and_velocity(float x_v,float z_v);


extern WbDeviceTag motor[4];
extern const char *motor_name[];
extern WbDeviceTag steer[4];
extern const char *steer_name[];

extern double angle;//最终角度
extern double velocity;//最终速度
extern int get_data_count;//得到数据频率计数
extern double trajectory_point[2][point_num];//数据点数组
extern int curve_times;//曲线计算次数
extern double t0;//每条曲线初始时间
extern double target_t;//目标时间
extern double each_t;//每条曲线的时间长度
extern double t_start;//规划起始时间
#endif
