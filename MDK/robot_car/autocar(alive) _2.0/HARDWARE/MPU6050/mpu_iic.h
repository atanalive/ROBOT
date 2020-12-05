#ifndef __MPU_IIC_H
#define __MPU_IIC_H
#include "sys.h"
#include "delay.h"

//第一个配引脚，第二个配输入输出
//通过操作寄存器CRL直接操纵IO口的输入输出方向
/*输入配置浮空输入，输出配置50Mhz的通用推挽输出*/
//之所以用浮空输入：我觉得是因为，在浮空输入状态下，IO口的电平状态完全由输入决定。
#define MPU_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define MPU_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<28;}
//关于转换成32位再或  ？？？

//IO操作函数
#define MPU_IIC_SCL         PAout(2)
#define MPU_IIC_SDA         PAout(3)
#define MPU_READ_SDA        PAin(3)  
#define iic_delay()         delay_us(2)

/*MPU的各种地址*/

//如果AD0脚(9脚)接地,IIC地址为0x68(不包含最低位)
//如果接v3.3,则IIC地址为0x69(不包含最低位)
#ifndef MPU_ADDR
#define MPU_ADDR            0x68
#endif










void MPU_IIC_Init(void);
void MPU_IIC_Start(void);
void MPU_IIC_Stop(void);
u8 MPU_IIC_Wait_Ack(void);
void MPU_IIC_Ack(void);
void MPU_IIC_NAck(void);
void MPU_IIC_Send_Byte(u8 txd);
u8 MPU_IIC_Read_Byte(unsigned char ack);

u8 MPU_Write_Byte(u8 reg,u8 data);
u8 MPU_Read_Byte(u8 reg);	
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 * buf);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);

#endif


