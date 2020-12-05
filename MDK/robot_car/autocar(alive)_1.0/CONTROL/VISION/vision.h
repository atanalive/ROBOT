#ifndef __VISION_H
#define __VISION_H
#include "sys.h"

struct VISION_TYPE
{
	u8 draw_order;//抽签的次序
	u8 queue_order;//排列次序
	u8 receive;//接收到的数据
	u8 grab;//抓取
	u8 put;//放置
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

