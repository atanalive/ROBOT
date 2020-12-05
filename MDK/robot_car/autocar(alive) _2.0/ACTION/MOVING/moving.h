#ifndef __MOVING_H
#define __MOVING_H
#include "sys.h"
#include "path.h"


u8 right_double_online_track(u8 v1,u8 v2);
u8 front_online_track(u8 v1,u8 v2);
u8 right_count_black_track(u8 count);
u8 front_count_black_track(u8 count);
u8 delay_moving(u32 delay_time);
//��ʼ˫�ߵ��м� v1�����ٶ� v2�����ٶ�
void start_to_middle_moving(u8 v1,u8 v2,u8 count);
//��ʼ·�η�װ���� 
void middle_to_grab_moving(u8 vx,u8 vy,u32 delay1,u32 delay2,enum LOCATION_PATH target);
//ץȡ·�ι滮��װ���� countΪ���������Ĵ�С v1Ϊ���ٴ�С v2Ϊ���ٴ�С 
void grap_to_put_moving(u8 count,u8 v1,u8 v2,enum LOCATION_PATH target);
//�ص�����̨ץȡ v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
void return_to_grab_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u32 delay3,u32 delay4,enum LOCATION_PATH target);
//�ص���ʼ�� v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
void return_to_start_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u32 delay3,u32 delay4,enum LOCATION_PATH target);



void coordinates_to_wheelvelocity_changeover(void);

#endif



