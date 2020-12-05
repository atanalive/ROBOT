#include "mpu_iic.h"

/************alive*************/

//�����ܽ�iic����ԭ��
/*��������ʱֻ��SDA��ȫ��SCL�ĸߵ�ƽ�ڲ��ܷ���
Ҳ����˵SCLΪ�ߣ���Ϊ���У����о��������ݲ���
��ʱ����SDAΪ�ȶ��ĵ͵�ƽ�ͷ��͵͵�ƽ����֮�ߵ�ƽ
��SCLΪ�͵�ƽʱ��ǯס���ߣ������������
*/

//ע�⣺��©�����SDA��SCLҪ����������

//PB6  PB7
//��ʼ��IIC
//SDA:��������
//SCL:����ʱ��
void MPU_IIC_Init(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;//��ʱ�����������������˵�ÿ�©��������������Ƚϰ�ȫ��why����
	gpio.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);//����Ϊ�ߵ�ƽ
	
}

//START:when CLK is high,DATA change form high to low
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_SCL=1;
  iic_delay();
	MPU_IIC_SDA=0;
	iic_delay();
	MPU_IIC_SCL=0;
	//��SCLΪ�ߵ�ƽʱ��ʾ���У����͵�ƽʱǯס����
  //�������һ�б�ʾǯס���ߣ�׼����������
}


//STOP:when CLK is high DATA change form low to high
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT()
	MPU_IIC_SCL=0;
	MPU_IIC_SDA=0;
  iic_delay();
	MPU_IIC_SCL=1;
	MPU_IIC_SDA=1;
  iic_delay();}

/*
���Ȱ�SDA,SCL���ߣ����һֱΪ�ߣ�˵�����յ��Ƿ�Ӧ���ź�
(NACK���ߴ������ʧ��)
���SDA����Ϊ�͵�ƽ������յ���Ӧ���źţ�Ȼ���SCL����
*/
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(void)
{
	u8 waittime=0;
	MPU_SDA_IN();
	MPU_IIC_SDA=1;  iic_delay();
	MPU_IIC_SCL=1;  iic_delay();
	while(MPU_READ_SDA)
  {
		waittime++;
		if(waittime>250)
    {
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL=0;//ǯס���ߣ���ֹ�ҷ����ݡ�����֪�����Ҳµģ�
	return 0;
}



//�������غ��½��طֱ��Ӧstop��start

//����Ӧ��
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL=0;//��ǯס�ٻ�����ֹ��������Ӱ��(ä��)
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
  iic_delay();
	MPU_IIC_SCL=1;//�����͵�ƽ��ȥ
  iic_delay();
	MPU_IIC_SCL=0;
}

void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	iic_delay();
	MPU_IIC_SCL=1;//�����ߵ�ƽ��ȥ
	iic_delay();
	MPU_IIC_SCL=0;
	
}

//����һ���ֽڣ���λ����
void MPU_IIC_Send_Byte(u8 txd)
{
	MPU_SDA_OUT();
	MPU_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	for(u8 t=0;t<8;t++)
  {
		MPU_IIC_SDA=(txd&0x80)>>7;
		MPU_IIC_SCL=1;
		iic_delay();
		MPU_IIC_SCL=0;
		iic_delay();
	}
	
}

//��ȡһ���ֽڣ��ȶ����Ǹ�λ
u8 MPU_IIC_Read_Byte(unsigned char ack)//no symbol type:8 vyte
{
	unsigned char receive=0;
	MPU_SDA_IN();
	for(u8 i=0;i<8;i++)
	{
		MPU_IIC_SCL=0;
		iic_delay();
		MPU_IIC_SCL=1;//��Ϊ�ߵ�ƽ��ʼ�������ݣ�����ä�£�
		receive<<=1;
		if(MPU_READ_SDA)
    {
			receive+=1;
		}
		iic_delay();
	}
	if(ack) MPU_IIC_Ack();
	else    MPU_IIC_NAck();
	return receive;	
}

