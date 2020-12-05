#ifndef MYIIC_H
#define MYIIC_H
#include "sys.h"
#include "delay.h"

//第一个配引脚，第二个配输入输出
//通过操作寄存器CRL直接操纵IO口的输入输出方向
/*输入配置浮空输入，输出配置50Mhz的通用推挽输出*/
//之所以用浮空输入：我觉得是因为，在浮空输入状态下，IO口的电平状态完全由输入决定。
#define SDA_IN()  {GPIOC->CRL&=0XFFFFFF0F;GPIOC->CRL|=(u32)8<<4;}//浮空输入
#define SDA_OUT() {GPIOC->CRL&=0XFFFFFF0F;GPIOC->CRL|=(u32)3<<4;}//推挽输出50Mhz
//关于转换成32位再或  ？？？

//IO操作函数
#define IIC_SCL         PCout(0)
#define IIC_SDA         PCout(1)
#define READ_SDA        PCin(1)
#define iic_delay()     delay_us(2)



void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);

u8 Write_Byte(u8 reg,u8 data);
u8 Read_Byte(u8 reg);	
u8 Write_Len(u8 addr,u8 reg,u8 len,u8 * buf);
u8 Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);

#endif


