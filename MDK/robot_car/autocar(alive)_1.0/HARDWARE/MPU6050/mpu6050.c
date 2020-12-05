#include "mpu6050.h"
#include "mpu_iic.h"
#include "math.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "main.h"

/************alive*************/

u8 mpu6050_init(void)
{
	u8 res;
	MPU_IIC_Init();
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x80);//��λMPU6050(bit7Ϊ1��λ����λ������mpuӲ���Զ������λ)
	delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x00);//����MPU6050(bit6 SLEEP �ϵ縴λ��Ϊ1(��Ҫ�ϵ縴λ��·��)�����㻽��
	
	MPU_Set_Gyro_Fsr(3);//�����Ǵ�����,+-2000dps
	MPU_Set_Accel_Fsr(0);//���ٶȴ�����,+-2g
	MPU_Set_Rate(50);//���ò�����Ϊ50Hz
	
	MPU_Write_Byte(MPU_INT_EN_REG,0x00);//�ر������ж�(interrupt:�ж�)
	MPU_Write_Byte(MPU_USER_CTRL_REG,0x00);//IIC��ģʽ�ر�
	MPU_Write_Byte(MPU_FIFO_EN_REG,0x00);//�ر�FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0x80);//INT���ŵ͵�ƽ��Ч(why?)
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);//��ȡ����ID:This register is used to verify the identity of the device
	if(res==MPU_ADDR)//�����ַ��ȷ
  {
		MPU_Write_Byte(MPU_SIGPATH_RST_REG,0x01);//����CLKSEL,PLL X��Ϊ�ο�
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0x00);//���ٶ��������Ƕ�����,bit0~5�����Ƿ�������ģʽ����Ϊ0���乤����
		//MPU_Set_Rate(50);//why?
	}else return 1;
	return 0;
}

//���������Ǵ�����
//mode:  0,+-250dps;  1,+-500dps;  2,+-1000dps;  3,+-2000dps
u8 MPU_Set_Gyro_Fsr(u8 mode)//fsr:full scale range �����̷�Χ
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,mode<<3);
}

//���ü��ٶȴ�����
//mode:  0,+-2g;  1,+-4g;  2,+-8g;  3,+-16g
u8 MPU_Set_Accel_Fsr(u8 mode)//accle:������
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,0<<3);
}

//���õ�ͨ�˲���(���ٶȴ���������)
u8 MPU_Set_LPF(u16 lpf)//LPF:Low Pass Filter ��ͨ�˲���
{
	u8 data;
	if(lpf>188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
	return MPU_Write_Byte(MPU_CFG_REG,data);
}


//���ò�����(����fs(���������Ƶ��)Ϊ1KHz)
//rate:4~1000(Hz)
//����Ƶ��=���������Ƶ��(1khz)/(1+SMPLRT_DIV(��Ƶϵ��))
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;//��Ƶϵ��
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);
	return MPU_Set_LPF(rate/2);//�˲�Ƶ��һ��Ϊ�����ʵ�һ��
	
}

//��ȡ�����ǵ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:   0:����   1:����
u8 MPU_Get_Gyroscope(u16 *gx,u16 *gy,u16 *gz)//gyroscope:������
{
	u8 buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
  {
		*gx=((((u16)buf[0])<<8)|buf[1]);
		*gy=((((u16)buf[2])<<8)|buf[3]);
		*gz=((((u16)buf[4])<<8)|buf[5]);
	}
	return res;
}

//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:  0:����  1:����
u8 MPU_Get_Accelerometer(u16 *ax,u16 *ay,u16 *az)//Accelerometer:���ټ�
{
	u8 buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
  {
		*ax=((((u16)buf[0])<<8)|buf[1]);
		*ay=((((u16)buf[2])<<8)|buf[3]);
		*az=((((u16)buf[4])<<8)|buf[5]);
	}
	return res;
}

//�õ��¶�ֵ
//����ֵ:  0:����  1:����
void MPU_Get_Temperature(u16 *read)
{
	u8 buf[2],res;
	u16 raw=0;
	float temp;
	res=MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
	if(res==0)
  {
		raw=((((u16)buf[0])<<8)|buf[1]);
	}
  temp=36.53+((double)raw)/340;
	*read=temp*100;
}

/***************IICͨ��д���ȡ����********************/

/*****дһ���ֽ�*****/
//����0:����  ����1:����
//˼·:����������ַд����->д�Ĵ�����ַ->��������
u8 MPU_Write_Byte(u8 reg,u8 data)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+R/W(���λΪ1,�Ƕ�����,Ϊ0,��д����)
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	MPU_IIC_Send_Byte(reg);//д�Ĵ�����ַ
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}//�ȴ�Ӧ��
	MPU_IIC_Send_Byte(data);//��������
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	MPU_IIC_Stop();
	return 0;
}


/*****��һ���ֽ�*****/

//˼·:����������ַд����->д�Ĵ�����ַ->������ʼ�ź�->����������ַ������->��ȡ����
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����������ַ+д����	
	MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
    MPU_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//����������ַ+������	
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	res=MPU_IIC_Read_Byte(0);//��ȡ����,����nACK 
    MPU_IIC_Stop();			//����һ��ֹͣ���� 
	return res;		
}


/******����д******/

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 * buf)//buf:buffer(������)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	MPU_IIC_Send_Byte(reg);
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	for(u8 i=0;i<len;i++)
  {
	  MPU_IIC_Send_Byte(buf[i]);
		if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	}
	MPU_IIC_Stop();
	return 0;
}

/********������*********/
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	MPU_IIC_Send_Byte(reg);
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	for(u8 r=0;r<len;r++)
  {
		if(r==len-1)*buf=MPU_IIC_Read_Byte(0);
		else *buf=MPU_IIC_Read_Byte(1);
		buf++;
	}
	MPU_IIC_Stop();
	return 0;
}


/************��ú�ƫ��***********/
//�õ�dmp����������(ע��,��������Ҫ�Ƚ϶��ջ,�ֲ������е��)
//yaw:�����   ����:0.1��   ��Χ:-180.0��<---> +180.0��
//����ֵ:0,����
//    ����,ʧ��
u8 mpu_dmp_get_yaw_data(float *yaw)
{
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	short gyro[3], accel[3], sensors;
	unsigned char more;
	long quat[4]; 
	if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more))return 1;	 
	/* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
	 * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
	**/
	/*if (sensors & INV_XYZ_GYRO )
	send_packet(PACKET_TYPE_GYRO, gyro);
	if (sensors & INV_XYZ_ACCEL)
	send_packet(PACKET_TYPE_ACCEL, accel); */
	/* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
	 * The orientation is set by the scalar passed to dmp_set_orientation during initialization. 
	**/
	if(sensors&INV_WXYZ_QUAT) 
	{
		q0 = quat[0] / q30;	//q30��ʽת��Ϊ������
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}

/************��ú�ƫ�ǵĽ��ٶ�(������)***********/
float get_yaw_angular_velocity(void)
{
	
	u8 buf[2],res;
	u16 gz;
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_ZOUTH_REG,2,buf);
	if(res==0)
  {
		gz=((((u16)buf[0])<<8)|buf[1]);
	}
	return gz*pi/(180.0f*16.40f);
}






