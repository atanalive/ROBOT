#include "main.h"
#include "math.h"
#include "moving.h"
#include "path.h"
#include "fsm.h"
#include "usart.h"
#include "encoder.h"
#include "feedback.h"

float theta;
static u8 a=1;
static u32 time_control=0;

//���������ٶ�ת���������ٶ�
void coordinates_to_wheelvelocity_changeover(void)
{
	if(a==1){ROBOT_NOW.initial_yaw=ROBOT_NOW.yaw;a++;}//�õ���ʼ�Ƕ�
  theta=pi*(ROBOT_NOW.yaw-ROBOT_NOW.initial_yaw)/180.0f;//תΪ�����ƣ�pi������main.h�ļ���
	
	encoder.target_velocity[0]=sin(theta)*ROBOT_NOW.target_Vx-cos(theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[1]=sin(pi/3-theta)*ROBOT_NOW.target_Vx+cos(pi/3-theta)+car_length*ROBOT_NOW.target_w;
	encoder.target_velocity[2]=(-sin(pi/3+theta))*ROBOT_NOW.target_Vx+cos(pi/3+theta)*ROBOT_NOW.target_Vy+car_length*ROBOT_NOW.target_w;
}

//ѭ��·���滮
void moving_track()
{
	switch(PATH_NOW.now_path)
  {
		case stanby:
			ROBOT_NOW.Vx=0;
		  ROBOT_NOW.Vy=0;
			break;
		case calibration://������ʼ��
			ROBOT_NOW.target_Vy=-40;
		  if(ROBOT_NOW.Vy>=-35)
			{
				mpu_dmp_get_yaw_data(&ROBOT_NOW.initial_yaw);//������ʼ��
				ROBOT_CALIBRATION=CALIBRATION_SUCCEED;

			}
			break;
		case start_to_middle://��ʼ���м�
			start_to_middle_moving(50,30,3);
			
			break;
		case middle_to_grab://�м䵽����̨ǰ׼��ץȡ
			switch(PATH_NOW.next_location)
			{
				case path_right:	
					middle_to_grab_moving(30,-30,500,1200,path_right);//vx vy delay1 delay2
				  break;
				case path_left:
				  middle_to_grab_moving(-30,-30,500,1200,path_left);
				  break;
				case path_middle:
					middle_to_grab_moving(0,-30,0,700,path_middle);
					break;
			}
			break;
		case grab_to_put:
		{
			u8 v1=50;//�����ٶ�
		  u8 v2=30;//�����ٶ�
			switch(PATH_NOW.now_location)
			{
				case path_left:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							grap_to_put_moving(0,-v1,v2,path_left);//black_count vhigh vlow
							break;
						case path_right:
							grap_to_put_moving(1,v1,v2,path_right);
							break;
						case path_middle:
							grap_to_put_moving(0,v1,v2,path_middle);
							break;
					}
					break;
				case path_right:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							grap_to_put_moving(1,-v1,v2,path_left);
							break;
						case path_right:
							grap_to_put_moving(0,v1,v2,path_right);
							break;
						case path_middle:
							grap_to_put_moving(0,-v1,v2,path_middle);
							break;
					}
				case path_middle:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							grap_to_put_moving(0,-v1,v2,path_left);
							break;
						case path_right:
							grap_to_put_moving(1,v1,v2,path_right);
							break;
						case path_middle:
							grap_to_put_moving(0,-v1,v2,path_middle);
							break;
					}
					break;
			}
		}
		break;
		case return_to_grab:
		{
			u8 v1=50;
			u8 v2=30;
			u8 v3=30;
			u8 v4=30;
			u32 delay3=500;
			u32 delay4=1200;
			switch(PATH_NOW.now_location)
			{
				case path_left:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							//�ص�����̨ץȡ v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
							return_to_grab_moving(v1,v2,0,v3,-v4,delay3,delay4,path_left);
						case path_right:
							return_to_grab_moving(v1,v2,1,v3,-v4,delay3,delay4,path_right);
						case path_middle:
							return_to_grab_moving(v1,v2,1,0,-v4,0,delay4-delay3,path_middle);
					}
				case path_right:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							//�ص�����̨ץȡ v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
							return_to_grab_moving(-v1,v2,1,-v3,-v4,delay3,delay4,path_left);
						case path_right:
							return_to_grab_moving(-v1,v2,0,-v3,-v4,delay3,delay4,path_right);
						case path_middle:
							return_to_grab_moving(-v1,v2,1,0,-v4,0,delay4-delay3,path_middle);
					}
				case path_middle:
					switch(PATH_NOW.next_location)
					{
						case path_left:
							//�ص�����̨ץȡ v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
							return_to_grab_moving(v1,v2,0,-v3,-v4,delay3,delay4,path_left);
						case path_right:
							return_to_grab_moving(v1,v2,0,v3,-v4,delay3,delay4,path_right);
						case path_middle:
							return_to_grab_moving(v1,v2,0,0,-v4,0,delay4-delay3,path_middle);
					}
			}
		}
		break;
		case return_to_start:
		{
			u8 v1=50;
			u8 v2=30;
			u8 v3=30;
			u8 v4=30;
			u32 delay3=500;
			u32 delay4=1200;
			switch(PATH_NOW.now_location)
			{
				case path_left:
						return_to_start_moving(-v1,v2,2,-v3,-v4,delay3,delay4,path_left);
				case path_right:
					  return_to_start_moving(-v1,v2,3,-v3,-v4,delay3,delay4,path_left);
				case path_middle:
						return_to_start_moving(-v1,v2,4,-v3,-v4,delay3,delay4,path_left);
			}
		}
		break;
	}
}

//��ʱ�ƶ�(��Ӱ�����������ʱ)
u8 delay_moving(u32 delay_time)
{
	if(time.taskcount-time_control>=delay_time)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//�ұߴ�����˫��ѭ������ v1�ƶ�������ٶȴ�С  v2�����ٶȴ�С
u8 right_double_online_track(u8 v1,u8 v2)
{
	if((feedback.infrared_right&0x07)==0x00)
	{
		ROBOT_NOW.target_Vy=v1;
	}
	else if((feedback.infrared_right&0x07)==0x01)//��ǰ��
	{
		ROBOT_NOW.target_Vy=v2;
	}
	else if((feedback.infrared_right&0x07)==0x04)//�����
	{
		ROBOT_NOW.target_Vy=-v2;
	}
	else if((feedback.infrared_right&0x07)==0x02)//ѹ��
	{
		ROBOT_NOW.target_Vy=0;
		return 1;
	}
	else if((feedback.infrared_right&0x07)==0x07)//ȫѹ��
	{
		ROBOT_NOW.target_Vy=0;
		return 2;
	}
	return 0;
}

//ǰ�洫��������ѭ������
u8 front_online_track(u8 v1,u8 v2)
{
	if((feedback.infrared_front&0x70)==0x00)
	{
		ROBOT_NOW.target_Vx=v1;
	}
	else if((feedback.infrared_front&0x70)==0x10)//������
	{
		ROBOT_NOW.target_Vx=v2;
	}
	else if((feedback.infrared_front&0x70)==0x40)//������
	{
		ROBOT_NOW.target_Vx=-v2;
	}
	else if((feedback.infrared_front&0x70)==0x20)//ѹ��
	{
		ROBOT_NOW.target_Vx=0;
		return 1;
	}
	else if((feedback.infrared_front&0x70)==0x70)//ȫѹ��
	{
		ROBOT_NOW.target_Vx=0;
		return 2;
	}
	return 0;
}

//�ұߴ��������ߴ�������(�м������������жϹ��ߴ���)
u8 right_count_black_track(u8 count)
{
	if((feedback.infrared_right&0x08)==0x08)
	{
		feedback.right_black_count++;
	}
	if(feedback.right_black_count>=count)
		{
			return 1;
		}
	else{return 0;}
}

//ǰ�洫�������ߴ������٣������һ����������
u8 front_count_black_track(u8 count)
{
	if((feedback.infrared_front&0x08)==0x08)
	{
		feedback.front_black_count++;
		
	}
	if(feedback.front_black_count>=count)
		{
			return 1;
		}
	else{return 0;}
}


//��ʼ˫�ߵ��м� v1�����ٶ� v2�����ٶ�
void start_to_middle_moving(u8 v1,u8 v2,u8 count)
{
	if(right_double_online_track(v1,v2))
	{
		if(front_count_black_track(count))
		{
			if(front_online_track(v1,v2))
			{
				ROBOT_TRACKING=TRACKING_SUCCEED;
				time_control=time.taskcount;
			}
		}
		else
		{
			ROBOT_NOW.target_Vx=50;
		}
	}
}

//��ʼ·�η�װ���� vxΪx���ٶ� vyΪy���ٶ� delay1Ϊx��λ����ʱ delay2Ϊy��λ����ʱ
void middle_to_grab_moving(u8 vx,u8 vy,u32 delay1,u32 delay2,enum LOCATION_PATH target)
{
	if(delay_moving(delay1))
	{
		ROBOT_NOW.target_Vx=0;
		if(delay_moving(delay2))
		{
			time_control=time.taskcount;//���¸�ֵ���Ա��´�ʹ��
			mpu_dmp_get_yaw_data(&ROBOT_NOW.initial_yaw);//������ʼ��
			
			ROBOT_TRACKING=TRACKING_SUCCEED;
			PATH_NOW.now_location=target;
		}
		else{ROBOT_NOW.target_Vy=vy;}
	}
	else{ROBOT_NOW.target_Vx=vx;}
}
	
//ץȡ·�ι滮��װ���� countΪ���������Ĵ�С v1Ϊ���ٴ�С v2Ϊ���ٴ�С 
void grap_to_put_moving(u8 count,u8 v1,u8 v2,enum LOCATION_PATH target)
{
	if(front_count_black_track(count))
	{
		if(front_online_track(v1,v2))
		{
			if(right_count_black_track(2))
			{
				ROBOT_NOW.target_Vy=10;
				if(front_online_track(v1,v2)==2)
				{
					ROBOT_NOW.target_Vy=0;
					ROBOT_TRACKING=TRACKING_SUCCEED;
					PATH_NOW.now_location=target;
				}
			}
			else{ROBOT_NOW.target_Vy=50;}
		}
	}
	else{ROBOT_NOW.target_Vy=v1;}
}

//�ص�����̨ץȡ v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
void return_to_grab_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u32 delay3,u32 delay4,enum LOCATION_PATH target)
{
	if(right_count_black_track(1))//��һ��,����
	{
		if(right_count_black_track(2))//�ڶ���,����
		{
			if(right_double_online_track(30,30))//�ع�˫��
			{
				start_to_middle_moving(v1,v2,count);
				middle_to_grab_moving(v3,v4,delay3,delay4,target);
			}
		}
		else{ROBOT_NOW.target_Vy=-30;}
	}
	else{ROBOT_NOW.target_Vy=-50;}
}

//�ص���ʼ�� v1Ϊˮƽ�ƶ������ٶ� v2ˮƽ�ƶ������ٶ� count����Ҫѹ�ĸ��� v3ˮƽ�ٶ� v4��ֱ�ٶ� delay3ˮƽ��ʱ delay4��ֱ��ʱ
void return_to_start_moving(u8 v1,u8 v2,u8 count,u8 v3,u8 v4,u32 delay3,u32 delay4,enum LOCATION_PATH target)
{
	return_to_grab_moving(v1,v2,count,v3,v4,delay3,delay4,target);
}