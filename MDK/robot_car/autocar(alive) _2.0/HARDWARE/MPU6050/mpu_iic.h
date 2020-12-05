#ifndef __MPU_IIC_H
#define __MPU_IIC_H
#include "sys.h"
#include "delay.h"

//��һ�������ţ��ڶ������������
//ͨ�������Ĵ���CRLֱ�Ӳ���IO�ڵ������������
/*�������ø������룬�������50Mhz��ͨ���������*/
//֮�����ø������룺�Ҿ�������Ϊ���ڸ�������״̬�£�IO�ڵĵ�ƽ״̬��ȫ�����������
#define MPU_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define MPU_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<28;}
//����ת����32λ�ٻ�  ������

//IO��������
#define MPU_IIC_SCL         PAout(2)
#define MPU_IIC_SDA         PAout(3)
#define MPU_READ_SDA        PAin(3)  
#define iic_delay()         delay_us(2)

/*MPU�ĸ��ֵ�ַ*/

//���AD0��(9��)�ӵ�,IIC��ַΪ0x68(���������λ)
//�����v3.3,��IIC��ַΪ0x69(���������λ)
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


