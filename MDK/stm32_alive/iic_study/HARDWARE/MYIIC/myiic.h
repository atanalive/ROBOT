#ifndef MYIIC_H
#define MYIIC_H
#include "sys.h"
#include "delay.h"

//��һ�������ţ��ڶ������������
//ͨ�������Ĵ���CRLֱ�Ӳ���IO�ڵ������������
/*�������ø������룬�������50Mhz��ͨ���������*/
//֮�����ø������룺�Ҿ�������Ϊ���ڸ�������״̬�£�IO�ڵĵ�ƽ״̬��ȫ�����������
#define SDA_IN()  {GPIOC->CRL&=0XFFFFFF0F;GPIOC->CRL|=(u32)8<<4;}//��������
#define SDA_OUT() {GPIOC->CRL&=0XFFFFFF0F;GPIOC->CRL|=(u32)3<<4;}//�������50Mhz
//����ת����32λ�ٻ�  ������

//IO��������
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


