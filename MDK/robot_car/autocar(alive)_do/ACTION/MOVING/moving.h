#ifndef __MOVING_H
#define __MOVING_H
#include "sys.h"
#include "path.h"


u8 right_double_online_track(u8 v1,u8 v2);
u8 behind_online_track(u8 v1,u8 v2);
u8 right_count_black_track(u8 count);
u8 behind_count_black_track(u8 count);
u8 task_delay_ms(u32 delay_time);
//��ʼ˫�ߵ��м� v1�����ٶ� v2�����ٶ�
void start_to_middle_moving(u8 v1,u8 v2,u8 count);
//��ʼ·�η�װ���� 
void middle_to_grab_moving(u8 vx,u8 vy,u16 delay1,u16 delay2,enum LOCATION_PATH target);
//ץȡ·�ι滮��װ���� countΪ���������Ĵ�С v1Ϊ���ٴ�С v2Ϊ���ٴ�С 
void grap_to_put_moving(u8 count,u8 v1,u8 v2,u16 delay1,enum LOCATION_PATH target);
//�ص�����̨ץȡ v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
void return_to_grab_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u16 delay3,u16 delay4,enum LOCATION_PATH target);
//�ص���ʼ�� v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
void return_to_start_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u16 delay3,u16 delay4,enum LOCATION_PATH target);



void coordinates_to_wheelvelocity_changeover(void);

#endif



