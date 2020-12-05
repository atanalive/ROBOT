#ifndef __TASK_H
#define __TASK_H
#include "sys.h"

void task_frequence_loop(void);
struct TIME
{
	u16 taskcount;
};

extern struct TIME time;
void CONTROL_VELOCITY(void);


#endif


