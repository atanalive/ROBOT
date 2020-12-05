#ifndef  __HC_SR04_
#define  __HC_SR04_
#include "sys.h"

#define  TRIG  PFout(12)
#define  ECHO  PFin(13)


void hc_sr04_init(void);
u16 get_hc_sr04_hightime(void);
u16 get_hc_sr04_distance(void);

#endif



