#include "main.h"
/************************************************
 ALIENTEK ս��STM32F103������ʵ��13
 TFTLCD��ʾʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
 float pitch,roll,yaw; 		//ŷ����
 int main(void)
 {	 
	u8 t=0,report=1;			//Ĭ�Ͽ����ϱ�
	u8 key;
	
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;					//�¶�	 
	
// 	u16 x=100;
//	u8 a=0;
//	u8 b=0;
	 
	u8 lcd_id[12];			//���LCD ID�ַ���*/
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	KEY_Init();
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	mg995_init(20000-1,72-1);
	timinit(36000-1,2-1);
	
	usmart_dev.init(72);		//��ʼ��USMART
	MPU_IIC_Init();//��ʼ��MPU6050
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣	
	MPU_Init();					//��ʼ��MPU6050
 	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	LCD_ShowString(30,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(30,70,200,16,16,"MPU6050 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2015/1/17"); 
	while(mpu_dmp_init())
 	{
		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}  
	LCD_ShowString(30,130,200,16,16,"MPU6050 OK");
	LCD_ShowString(30,150,200,16,16,"KEY0:UPLOAD ON/OFF");
	POINT_COLOR=BLUE;//��������Ϊ��ɫ 
 	LCD_ShowString(30,170,200,16,16,"UPLOAD ON ");	 
 	LCD_ShowString(30,200,200,16,16," Temp:    . C");	
 	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");	 
 	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
			report=!report;
			if(report)LCD_ShowString(30,170,200,16,16,"UPLOAD ON ");
			else LCD_ShowString(30,170,200,16,16,"UPLOAD OFF");
		}
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			if(report)mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
			if(report)usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
			if((t%10)==0)
			{ 
				if(temp<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,200,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,200,temp/100,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,200,temp%10,1,16);		//��ʾС������ 
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,220,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,220,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,220,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,220,temp%10,1,16);		//��ʾС������ 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,240,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,240,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,240,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,240,temp%10,1,16);		//��ʾС������ 
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,260,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,260,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,260,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,260,temp%10,1,16);		//��ʾС������  
				t=0;
				//LED0=!LED0;//LED��˸
			}
		}
		t++; 
	} 	
/*
while(1){
	a=(b==0)?a:b;
  while((b=KEY_Scan(0))==0){
	LCD_ShowString(30,50,200,24,24,"ALIVE"); 
	LCD_ShowString(30,80,200,16,16,"l have      apples !");
  LCD_ShowNum(80,80,x,5,16);
	switch(a)
	{
		case 0:
			break;
		case 1:
			x--;
		LED0=1;
		LED1=0;
			break;
		case 2:
		LED0=1;
		LED1=1;
			break;
		case 3:
			x++;
		LED0=0;
		LED1=1;
			break;
		case 4:
			x=0;
		LED0=0;
		LED1=0;
			break;
		default:
		LED0=1;
		LED1=1;
			break;
	}
	}
	}*/
}

