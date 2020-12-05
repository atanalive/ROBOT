#include "pca9685.h"
#include "usart.h"

//alive ��װ
//pca9685��OE��һ��Ҫ�ӵ�

void pca_write(u8 adrr,u8 data)//��PCAд����,adrrd��ַ,data����
{ 
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	
	IIC_Stop();
}

u8 pca_read(u8 adrr)//��PCA������
{
	u8 data;
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(adrr);
	IIC_Wait_Ack();
	
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr|0x01);
	IIC_Wait_Ack();
	
	data=IIC_Read_Byte(0);
	IIC_Stop();
	
	return data;
}


void pca_setfreq(float freq)//����PWMƵ��
{
	u8 prescale,oldmode,newmode;
	double prescaleval;
	freq *= 0.92; 
	prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;
	prescale =floor(prescaleval + 0.5f);//��������

	oldmode = pca_read(pca_mode1);//pca_mode1 0x0

	newmode = (oldmode&0x7F)|0x10; // sleep  0x0&0x7f=0x0  0x0|0x10=0x10

	pca_write(pca_mode1, newmode); // go to sleep  D4sleepд1

	pca_write(pca_pre, prescale); // set the prescaler 
	//pca_pre 0xFE�������ڵļĴ��������������Ԥװ��ֵ����

	pca_write(pca_mode1, oldmode);//oldmode 0x0,D4д0�˳�sleepģʽ
	delay_ms(2);

	pca_write(pca_mode1, oldmode | 0xa1); 
	//0x0|0xa1=10100001 D6ʹ���ڲ�ʱ�ӣ�D5�ڲ���ַ��д���Զ����ӣ�D0��Ӧ0x70ͨ��i2c��ַ
}

void pca_setpwm(u8 num, u32 on, u32 off)
{
	pca_write(LED0_ON_L+4*num,on);
	pca_write(LED0_ON_H+4*num,on>>8);
	pca_write(LED0_OFF_L+4*num,off);
	pca_write(LED0_OFF_H+4*num,off>>8);
}

/*num:���PWM�������0~15��on:PWM��������ֵ0~4096,off:PWM�½�����ֵ0~4096
һ��PWM���ڷֳ�4096�ݣ���0��ʼ+1�������Ƶ�onʱ����Ϊ�ߵ�ƽ������������offʱ
����Ϊ�͵�ƽ��ֱ������4096���¿�ʼ�����Ե�on������0ʱ������ʱ,��on����0ʱ��
off/4096��ֵ����PWM��ռ�ձȡ�*/

//���ö���Ƕ� ģ����50hz ���ֶ��330hz
void set_steer_angle(u8 num,float angle)
{
	u32 on=0;
	u32 off;
	off=((angle*2/180+0.5)/20)*4096;
	printf("off is :%u",off);
	pca_setpwm(num,on,off);
}

void pca_init(float freq)
{
	IIC_Init();
	pca_write(pca_mode1,0x0);
	pca_setfreq(freq);
}

