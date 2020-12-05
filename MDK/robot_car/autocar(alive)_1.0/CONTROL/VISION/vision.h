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
};

enum VISION_SEND_TYPE
{
	VISION_ASK_JUDGEMENT_MODE=1,
	VISION_ASK_GRAB_MODE,
	VISION_ASK_PUT_MODE,
	VISION_DO_NOTHING,
};
extern enum VISION_SEND_TYPE VISION_SEND_CONTROL;

extern struct VISION_TYPE VISION;
void calculate_grab_and_put_order(void);
void analytic_data_in_vision(void);

#endif

