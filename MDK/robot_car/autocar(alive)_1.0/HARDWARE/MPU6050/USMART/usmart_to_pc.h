#ifndef __USMART_TO_PC
#define __USMART_TO_PC
#include  "sys.h"

void usart1_send_char(u8 c);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);

#endif


