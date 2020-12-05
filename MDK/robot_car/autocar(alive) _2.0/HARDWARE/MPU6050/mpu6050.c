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
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x80);//复位MPU6050(bit7为1则复位，复位结束后mpu硬件自动清零该位)
	delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0x00);//唤醒MPU6050(bit6 SLEEP 上电复位后为1(需要上电复位电路？)，清零唤醒
	
	MPU_Set_Gyro_Fsr(3);//陀螺仪传感器,+-2000dps
	MPU_Set_Accel_Fsr(0);//加速度传感器,+-2g
	MPU_Set_Rate(50);//设置采样率为50Hz
	
	MPU_Write_Byte(MPU_INT_EN_REG,0x00);//关闭所有中断(interrupt:中断)
	MPU_Write_Byte(MPU_USER_CTRL_REG,0x00);//IIC主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0x00);//关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0x80);//INT引脚低电平有效(why?)
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);//读取器件ID:This register is used to verify the identity of the device
	if(res==MPU_ADDR)//如果地址正确
  {
		MPU_Write_Byte(MPU_SIGPATH_RST_REG,0x01);//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0x00);//加速度与陀螺仪都工作,bit0~5控制是否进入待机模式，清为0让其工作。
		//MPU_Set_Rate(50);//why?
	}else return 1;
	return 0;
}

//设置陀螺仪传感器
//mode:  0,+-250dps;  1,+-500dps;  2,+-1000dps;  3,+-2000dps
u8 MPU_Set_Gyro_Fsr(u8 mode)//fsr:full scale range 满量程范围
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,mode<<3);
}

//设置加速度传感器
//mode:  0,+-2g;  1,+-4g;  2,+-8g;  3,+-16g
u8 MPU_Set_Accel_Fsr(u8 mode)//accle:加速器
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,0<<3);
}

//设置低通滤波器(角速度传感器带宽)
u8 MPU_Set_LPF(u16 lpf)//LPF:Low Pass Filter 低通滤波器
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


//设置采样率(假设fs(陀螺仪输出频率)为1KHz)
//rate:4~1000(Hz)
//采样频率=陀螺仪输出频率(1khz)/(1+SMPLRT_DIV(分频系数))
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;//分频系数
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);
	return MPU_Set_LPF(rate/2);//滤波频率一般为采样率的一半
	
}

//获取陀螺仪的值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始数据(带符号)
//返回值:   0:正常   1:出错
u8 MPU_Get_Gyroscope(u16 *gx,u16 *gy,u16 *gz)//gyroscope:陀螺仪
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

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读书(带符号)
//返回值:  0:正常  1:出错
u8 MPU_Get_Accelerometer(u16 *ax,u16 *ay,u16 *az)//Accelerometer:加速计
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

//得到温度值
//返回值:  0:正常  1:出错
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

/***************IIC通信写入读取数据********************/

/*****写一个字节*****/
//返回0:正常  返回1:出错
//思路:发送器件地址写命令->写寄存器地址->发送数据
u8 MPU_Write_Byte(u8 reg,u8 data)
{
	MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+R/W(最低位为1,是读操作,为0,是写操作)
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	MPU_IIC_Send_Byte(reg);//写寄存器地址
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}//等待应答
	MPU_IIC_Send_Byte(data);//发送数据
	if(MPU_IIC_Wait_Ack()){MPU_IIC_Stop();return 1;}
	MPU_IIC_Stop();
	return 0;
}


/*****读一个字节*****/

//思路:发送器件地址写命令->写寄存器地址->发送起始信号->发送器件地址读命令->读取数据
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    MPU_IIC_Start(); 
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//发送器件地址+写命令	
	MPU_IIC_Wait_Ack();		//等待应答 
    MPU_IIC_Send_Byte(reg);	//写寄存器地址
    MPU_IIC_Wait_Ack();		//等待应答
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//发送器件地址+读命令	
    MPU_IIC_Wait_Ack();		//等待应答 
	res=MPU_IIC_Read_Byte(0);//读取数据,发送nACK 
    MPU_IIC_Stop();			//产生一个停止条件 
	return res;		
}


/******连续写******/

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 * buf)//buf:buffer(缓冲区)
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

/********连续读*********/
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


/************获得航偏角***********/
//得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
//yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
//返回值:0,正常
//    其他,失败
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
		q0 = quat[0] / q30;	//q30格式转换为浮点数
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30; 
		*yaw   = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;	//yaw
	}else return 2;
	return 0;
}

/************获得航偏角的角速度(弧度制)***********/
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






