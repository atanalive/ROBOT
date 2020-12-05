#ifndef __TASK_H
#define __TASK_H
#include "sys.h"

void task_frequence_loop(void);
struct TIME
{
	u32 taskcount;
};

extern struct TIME time;
void ACTION(void);
void CALMULATE(void);


#endif


