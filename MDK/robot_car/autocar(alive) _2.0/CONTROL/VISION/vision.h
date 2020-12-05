#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

struct VISION_TYPE
{
	u8 draw_order;//��ǩ�Ĵ���
	u8 queue_order;//���д���
	u8 receive;//���յ�������
	u8 grab;//ץȡ
	u8 put;//����
	u8 send_count;//����һ�����ݷ��͵Ĵ���
};

enum VISION_SEND_TYPE
{
	VISION_ASK_START=1,
	VISION_ASK_JUDGEMENT,
	VISION_ASK_PUT,
	VISION_DO_NOTHING,
};
extern enum VISION_SEND_TYPE VISION_SEND_CONTROL;

extern struct VISION_TYPE VISION;
void calculate_grab_and_put_order(void);
void analytic_data_in_vision(void);

#endif

