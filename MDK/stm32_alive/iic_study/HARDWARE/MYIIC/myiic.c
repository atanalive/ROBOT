#include "myiic.h"

/************alive*************/

//�����ܽ�iic����ԭ��
/*��������ʱֻ��SDA��ȫ��SCL�ĸߵ�ƽ�ڲ��ܷ���
Ҳ����˵SCLΪ�ߣ���Ϊ���У����о��������ݲ���
��ʱ����SDAΪ�ȶ��ĵ͵�ƽ�ͷ��͵͵�ƽ����֮�ߵ�ƽ
��SCLΪ�͵�ƽʱ��ǯס���ߣ������������
*/

//ע�⣺��©�����SDA��SCLҪ����������

//PB8  PB9
//��ʼ��IIC
//SDA:��������
//SCL:����ʱ��
void IIC_Init(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);*/
	
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;//��ʱ�����������������˵�ÿ�©��������������Ƚϰ�ȫ��why����
	gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);//����Ϊ�ߵ�ƽ
	
}

//START:when CLK is high,DATA change form high to low
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA=1;
	IIC_SCL=1;
  iic_delay();
	IIC_SDA=0;
	iic_delay();
	IIC_SCL=0;
	//��SCLΪ�ߵ�ƽʱ��ʾ���У����͵�ƽʱǯס����
  //�������һ�б�ʾǯס���ߣ�׼����������
}


//STOP:when CLK is high DATA change form low to high
void IIC_Stop(void)
{
	SDA_OUT()
	IIC_SCL=0;
	IIC_SDA=0;
  iic_delay();
	IIC_SCL=1;
	IIC_SDA=1;
  iic_delay();}

/*
���Ȱ�SDA,SCL���ߣ����һֱΪ�ߣ�˵�����յ��Ƿ�Ӧ���ź�
(NACK���ߴ������ʧ��)
���SDA����Ϊ�͵�ƽ������յ���Ӧ���źţ�Ȼ���SCL����
*/
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 waittime=0;
	SDA_IN();
	IIC_SDA=1;  iic_delay();
	IIC_SCL=1;  iic_delay();
	while(READ_SDA)
  {
		waittime++;
		if(waittime>250)
    {
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ǯס���ߣ���ֹ�ҷ����ݡ�����֪�����Ҳµģ�
	return 0;
}



//�������غ��½��طֱ��Ӧstop��start

//�����ߵ�ƽӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;//��ǯס�ٻ�����ֹ��������Ӱ��(ä��)
	SDA_OUT();
	IIC_SDA=0;
  iic_delay();
	IIC_SCL=1;//�����͵�ƽ��ȥ
  iic_delay();
	IIC_SCL=0;
}
//�����͵�ƽӦ��
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	iic_delay();
	IIC_SCL=1;//�����ߵ�ƽ��ȥ
	iic_delay();
	IIC_SCL=0;
	
}

//����һ���ֽڣ���λ����
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
  {
		IIC_SDA=(txd&0x80)>>7;
		txd<<=1;
		IIC_SCL=1;
		iic_delay();
		IIC_SCL=0;
		iic_delay();
	}
	
}

//��ȡһ���ֽڣ��ȶ����Ǹ�λ
u8 IIC_Read_Byte(unsigned char ack)//no symbol type:8 vyte
{
	u8 i;
	unsigned char receive=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		iic_delay();
		IIC_SCL=1;//��Ϊ�ߵ�ƽ��ʼ�������ݣ�����ä�£�
		receive<<=1;
		if(READ_SDA)
    {
			receive+=1;
		}
		iic_delay();
	}
	if(ack) IIC_Ack();
	else    IIC_NAck();
	return receive;	
}

