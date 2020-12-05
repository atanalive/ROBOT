#include "main.h"
//��ѹ�ߣ���ͨ��3v d
//ѹ��: ����ͨ��0.5v
//����Ϊ1 ����Ϊ0
u16 voltage[8]={0};
u8 value_old=0xff;//Ĭ��ȫ��

//ѭ��������濴����������Ϊ��ͨ��0-7
//ADC1 Channel 0~7 -> PA0~7
//�õ���������ֵ(������)
u8 get_value_infrared(void)
{
	u8 value=0;
	u8 i;
	for(i=0;i<8;i++)
	{
		voltage[i]=Get_Adc_Average(ADC_Channel_0+i,5);
		#if INFRARED_DEBUG
			printf("voltage[%d] is %d \r\n",i,voltage[i]);
		#endif
	}
	//printf("#################################\r\n");
	#if INFRARED_DEBUG//��ӡvalue�Ķ����� ����1
	printf("value is ");
	#endif
	for(i=8;i>0;i--)
	{
		if(voltage[i-1]>3000)
		{
			value+=1;
			#if INFRARED_DEBUG//��ӡvalue�Ķ����� ����2
			printf("%d",1);
			#endif
		}
		else
		{
			#if INFRARED_DEBUG//��ӡvalue�Ķ����� ����3
			printf("%d",0);
			#endif
		}
		if(i==1)break;//���һ�β�������
		value=value<<1;
	}
	#if INFRARED_DEBUG//��ӡvalue�Ķ����� ����4
	printf("  :%d\r\n",value);
	#endif
	
	return value;
	
}

void led_set(u8 value)
{
	LED7=value&0x01;
	value=value>>1;
	LED6=value&0x01;
	value=value>>1;
	LED5=value&0x01;
	value=value>>1;
	LED4=value&0x01;
	value=value>>1;
	LED3=value&0x01;
	value=value>>1;
	LED2=value&0x01;
	value=value>>1;
	LED1=value&0x01;
	value=value>>1;
	LED0=value&0x01;
}

//�ұߵĴ������������
u8 infrared_right_value_handle(void)
{
	u8 value=0;
	u8 value_return=0;
	value=get_value_infrared();
	led_set(value);
	if(value!=0xff)//�������ȫ��
	{
		if(((0x80&value)==0x80)&&((0x80&value_old)==0x00))
		{
			value_return|=0x08;//��ǰ��Ĵ�������->�׵Ĵ���
			#if INFRARED_DEBUG 
			printf("�ұ�  ����+1\r\n");
			#endif
		}
		else if(value==0x00)
		{
			value_return|=0x07;//ȫѹ��
			#if INFRARED_DEBUG 
			printf("�ұ�  ȫѹ��\r\n");
			#endif
		}
		else if((value&0x24)==0x00&&(value&0x42)==0x42)
		{
			value_return|=0x02;//��˫�ߣ�36�ڣ�27��
			#if INFRARED_DEBUG 
			printf("�ұ�  ��˫��\r\n");
			#endif
		}
		else if(value>0x40)
		{
			value_return|=0x01;//δ��λ ���ڵ�7λ
			#if INFRARED_DEBUG 
			printf("�ұ�  δ��λ\r\n");
			#endif
		}
		else if(value<0x20)
		{
			value_return|=0x04;//�ѳ��� С�ڵ���λ
			#if INFRARED_DEBUG 
			printf("�ұ�  �ѳ���\r\n");
			#endif
		}
		
		value_old=value;
		return value_return;
	}
	else//���ȫ��
	{
		#if INFRARED_DEBUG 
		printf("�ұ�  ȫ��\r\n");
		#endif
		return 0;
	}
} 


//����Ĵ������������
u8 infrared_front_value_handle(void)
{
	u8 value=0;
	u8 value_return=0;
	value=get_value_infrared();
	led_set(value);
	if(value!=0xff)//�������ȫ��
	{
		if(((0x80&value)==0x80)&&((0x80&value_old)==0))
		{
			//����ߵĴ�������->�׵Ĵ���
			value_return|=0x80;
			#if INFRARED_DEBUG 
			printf("����  ����+1\r\n");
			#endif
		}
		else if(value==0x00)
		{
			value_return|=0x70;//ȫѹ��
			#if INFRARED_DEBUG 
			printf("����  ȫѹ��\r\n");
			#endif
		}
		else if((value&0x18)==0x00&&(value&0x24)==0x24)
		{
			value_return|=0x20;//�����ߣ�45�ڣ�36��
			#if INFRARED_DEBUG 
			printf("����  ������\r\n");
			#endif
		}
		else if(value>0x20)
		{
			value_return|=0x40;//ƫ�� ���ڵ�6λ
			#if INFRARED_DEBUG 
			printf("����  ����\r\n");
			#endif
		}
		else if(value<0x10)
		{
			value_return|=0x10;//ƫ�� С�ڵ�5λ
			#if INFRARED_DEBUG 
			printf("����  ����\r\n");
			#endif
		}
		
		value_old=value;
		return value_return;
	}
	else//���ȫ��
	{
		#if INFRARED_DEBUG 
		printf("����  ȫ��\r\n");
		#endif
		return 0;
	}
}

