#include "mpu_iic.h"

/************alive*************/

//个人总结iic工作原理：
/*发送数据时只有SDA完全在SCL的高电平内才能发送
也就是说SCL为高，即为空闲，空闲就允许数据操作
此时，若SDA为稳定的低电平就发送低电平，反之高电平
而SCL为低电平时会钳住总线，不让其操作。
*/

//注意：开漏输出下SDA和SCL要接上拉电阻

//PB6  PB7
//初始化IIC
//SDA:串行数据
//SCL:串行时钟
void MPU_IIC_Init(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;//暂时先用推挽输出，但据说用开漏输出和上拉电阻会比较安全（why？）
	gpio.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);//设置为高电平
	
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
	//当SCL为高电平时表示空闲，当低电平时钳住总线
  //这里最后一行表示钳住总线，准备发送数据
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
首先把SDA,SCL拉高，如果一直为高，说明接收的是非应答信号
(NACK或者代表接收失败)
如果SDA被拉为低电平代表接收到了应答信号，然后把SCL拉低
*/
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
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
	MPU_IIC_SCL=0;//钳住总线，防止乱发数据。（不知道，乱猜的）
	return 0;
}



//而上升沿和下降沿分别对应stop和start

//产生应答
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL=0;//先钳住再换，防止其他不良影响(盲猜)
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
  iic_delay();
	MPU_IIC_SCL=1;//发个低电平过去
  iic_delay();
	MPU_IIC_SCL=0;
}

void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	iic_delay();
	MPU_IIC_SCL=1;//发个高电平过去
	iic_delay();
	MPU_IIC_SCL=0;
	
}

//发送一个字节，高位先行
void MPU_IIC_Send_Byte(u8 txd)
{
	MPU_SDA_OUT();
	MPU_IIC_SCL=0;//拉低时钟开始数据传输
	for(u8 t=0;t<8;t++)
  {
		MPU_IIC_SDA=(txd&0x80)>>7;
		MPU_IIC_SCL=1;
		iic_delay();
		MPU_IIC_SCL=0;
		iic_delay();
	}
	
}

//读取一个字节，先读的是高位
u8 MPU_IIC_Read_Byte(unsigned char ack)//no symbol type:8 vyte
{
	unsigned char receive=0;
	MPU_SDA_IN();
	for(u8 i=0;i<8;i++)
	{
		MPU_IIC_SCL=0;
		iic_delay();
		MPU_IIC_SCL=1;//设为高电平开始接收数据（个人盲猜）
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

