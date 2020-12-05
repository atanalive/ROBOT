#include "myiic.h"

/************alive*************/

//个人总结iic工作原理：
/*发送数据时只有SDA完全在SCL的高电平内才能发送
也就是说SCL为高，即为空闲，空闲就允许数据操作
此时，若SDA为稳定的低电平就发送低电平，反之高电平
而SCL为低电平时会钳住总线，不让其操作。
*/

//注意：开漏输出下SDA和SCL要接上拉电阻

//PB8  PB9
//初始化IIC
//SDA:串行数据
//SCL:串行时钟
void IIC_Init(void)
{
	GPIO_InitTypeDef gpio;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	/*
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);*/
	
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;//暂时先用推挽输出，但据说用开漏输出和上拉电阻会比较安全（why？）
	gpio.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1);//设置为高电平
	
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
	//当SCL为高电平时表示空闲，当低电平时钳住总线
  //这里最后一行表示钳住总线，准备发送数据
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
首先把SDA,SCL拉高，如果一直为高，说明接收的是非应答信号
(NACK或者代表接收失败)
如果SDA被拉为低电平代表接收到了应答信号，然后把SCL拉低
*/
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
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
	IIC_SCL=0;//钳住总线，防止乱发数据。（不知道，乱猜的）
	return 0;
}



//而上升沿和下降沿分别对应stop和start

//产生高电平应答
void IIC_Ack(void)
{
	IIC_SCL=0;//先钳住再换，防止其他不良影响(盲猜)
	SDA_OUT();
	IIC_SDA=0;
  iic_delay();
	IIC_SCL=1;//发个低电平过去
  iic_delay();
	IIC_SCL=0;
}
//产生低电平应答
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	iic_delay();
	IIC_SCL=1;//发个高电平过去
	iic_delay();
	IIC_SCL=0;
	
}

//发送一个字节，高位先行
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL=0;//拉低时钟开始数据传输
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

//读取一个字节，先读的是高位
u8 IIC_Read_Byte(unsigned char ack)//no symbol type:8 vyte
{
	u8 i;
	unsigned char receive=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		iic_delay();
		IIC_SCL=1;//设为高电平开始接收数据（个人盲猜）
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

