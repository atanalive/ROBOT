#ifndef __FSM_H
#define __FSM_H
#include "sys.h"


//ע�⣬ͷ�ļ�ֻ�ܷ����������ܷŶ��壬�Ŷ���Ļ��������ļ����û�����ظ�����Ĵ���
//��Ȼ�����ͱ����ǿ��Ե�
//���ֻ��һ��c�ã�h���Զ������
/*****************��������״̬*******************/
enum ROBOT_STATE_TYPE
{
	STANDBY,
	CALIBRATION,  //У׼
	START_TO_MIDDLE,
	MIDDLE_TO_GRAB,
	GRAB,
	VISION_JUDGEMENT,
	GRAB_TO_PUT,
	PUT,
	RETURN_TO_GRAB,
	RETURN_TO_START,	
};

extern enum ROBOT_STATE_TYPE ROBOT_STATE;//��ǰ״̬
/********************����������ʵʱ״̬**************************/
#define L  58/2                //���Ӱ뾶L(mm)
#define length    300          //����
#define width     210          //����
#define infrared  10           //���⴫����
#define car_length  100        //���������ĵľ���

struct ROBOT_NOW_TYPE
{
	float X;//(X,Y�ɼ����࣬ѭ��������)
	float Y;
	float yaw;//����ǣ��Ƕ���(�������ǻ��)
	float initial_yaw;//��ʼ�����(�Ƕ���)
	float Vx;//(���������������)
	float Vy;
	float w;//yaw���ٶȣ�������(�����ǻ��)
	float target_Vx;//(�ٶȹ滮���)(�����������µ�Ŀ���ٶ�)
	float target_Vy;
	float target_w;
	u8 grab_order;//ץȡ����(ͨ����ݮ�ɻ�õ����д���Ϳ��ػ�õĳ�ǩ�������õ�)
	u8 put_order;//�ڷŴ���
};

extern struct ROBOT_NOW_TYPE ROBOT_NOW;//����������ʵʱ״̬

//ץȡ�ͰڷŴ���涨:(�����ҷֱ�Ϊabc)
/*
1:abc 2:acb 3:bac 4:bca 5:cab 6:cba
*/
/*****************��������״̬******************/
struct state_control
{
  u8 TURNAROUND_TO_PUT_count;//TURNAROUND_TO_PUT״ִ̬�м���
  u8 TURNAROUND_TO_TABLE_count;//TURNAROUND_TO_TABLE״ִ̬�м���

};

extern struct state_control STATE_COUNT;//��������״̬
/*****************��������״̬********************/
//У׼״̬(У׼�ı��ʾ�������ȷ��������)
enum CALIBRATION_TYPE
{
	CALIBRATION_STOP,
	CALIBRATION_START,
	CALIBRATION_SUCCEED,
};

extern enum CALIBRATION_TYPE ROBOT_CALIBRATION;//У׼״̬
//�Ӿ�����(���ǿ��ؽ�����ݮ����Ϣ�ĺ���)
enum ROBOT_VISION_TYPE
{	
	VISION_STOP,
	VISION_START_RECEIVE,
	VISION_SUCCEED,
};

extern enum ROBOT_VISION_TYPE ROBOT_VISION;//�Ӿ�����
//ץȡ״̬
enum ROBOT_GRAB_TYPE
{
	GRAB_STOP,
	GRAB_START,
	GRAB_SUCCEED,
};

extern enum ROBOT_GRAB_TYPE ROBOT_GRAB;//ץȡ״̬
//Ͷ��
enum ROBOT_PUT_TYPE
{
	PUT_STOP,
	PUT_START,
	PUT_SUCCEED,
};

extern enum ROBOT_PUT_TYPE ROBOT_PUT;//Ͷ��
//�ƶ�
enum ROBOT_TRACKING_TYPE
{
	TRACKING_STOP,
	TRACKING_START,
	TRACKING_SUCCEED,
};

extern enum ROBOT_TRACKING_TYPE ROBOT_TRACKING;//�ƶ�


void FSM_WORK(void);
void fsm_init(void);
void infrared_init(void);

#endif

