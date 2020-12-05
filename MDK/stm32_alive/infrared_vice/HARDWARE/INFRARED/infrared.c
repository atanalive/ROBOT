#include "main.h"
//不压线：导通：3v d
//压线: 不导通：0.5v
//白线为1 黑线为0
u16 voltage[8]={0};
u8 value_old=0xff;//默认全白

//循迹板从上面看：从右往左为：通道0-7
//ADC1 Channel 0~7 -> PA0~7
//得到传感器的值(二进制)
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
	#if INFRARED_DEBUG//打印value的二进制 部分1
	printf("value is ");
	#endif
	for(i=8;i>0;i--)
	{
		if(voltage[i-1]>3000)
		{
			value+=1;
			#if INFRARED_DEBUG//打印value的二进制 部分2
			printf("%d",1);
			#endif
		}
		else
		{
			#if INFRARED_DEBUG//打印value的二进制 部分3
			printf("%d",0);
			#endif
		}
		if(i==1)break;//最后一次不用左移
		value=value<<1;
	}
	#if INFRARED_DEBUG//打印value的二进制 部分4
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

//右边的传感器情况处理
u8 infrared_right_value_handle(void)
{
	u8 value=0;
	u8 value_return=0;
	value=get_value_infrared();
	led_set(value);
	if(value!=0xff)//如果不是全白
	{
		if(((0x80&value)==0x80)&&((0x80&value_old)==0x00))
		{
			value_return|=0x08;//最前面的传感器黑->白的次数
			#if INFRARED_DEBUG 
			printf("右边  计数+1\r\n");
			#endif
		}
		else if(value==0x00)
		{
			value_return|=0x07;//全压线
			#if INFRARED_DEBUG 
			printf("右边  全压线\r\n");
			#endif
		}
		else if((value&0x24)==0x00&&(value&0x42)==0x42)
		{
			value_return|=0x02;//卡双线：36黑，27白
			#if INFRARED_DEBUG 
			printf("右边  卡双线\r\n");
			#endif
		}
		else if(value>0x40)
		{
			value_return|=0x01;//未到位 大于第7位
			#if INFRARED_DEBUG 
			printf("右边  未到位\r\n");
			#endif
		}
		else if(value<0x20)
		{
			value_return|=0x04;//已超过 小于第六位
			#if INFRARED_DEBUG 
			printf("右边  已超过\r\n");
			#endif
		}
		
		value_old=value;
		return value_return;
	}
	else//如果全白
	{
		#if INFRARED_DEBUG 
		printf("右边  全白\r\n");
		#endif
		return 0;
	}
} 


//后面的传感器情况处理
u8 infrared_front_value_handle(void)
{
	u8 value=0;
	u8 value_return=0;
	value=get_value_infrared();
	led_set(value);
	if(value!=0xff)//如果不是全白
	{
		if(((0x80&value)==0x80)&&((0x80&value_old)==0))
		{
			//最左边的传感器黑->白的次数
			value_return|=0x80;
			#if INFRARED_DEBUG 
			printf("后面  计数+1\r\n");
			#endif
		}
		else if(value==0x00)
		{
			value_return|=0x70;//全压线
			#if INFRARED_DEBUG 
			printf("后面  全压线\r\n");
			#endif
		}
		else if((value&0x18)==0x00&&(value&0x24)==0x24)
		{
			value_return|=0x20;//卡单线：45黑，36白
			#if INFRARED_DEBUG 
			printf("后面  卡单线\r\n");
			#endif
		}
		else if(value>0x20)
		{
			value_return|=0x40;//偏左 大于第6位
			#if INFRARED_DEBUG 
			printf("后面  往左\r\n");
			#endif
		}
		else if(value<0x10)
		{
			value_return|=0x10;//偏右 小于第5位
			#if INFRARED_DEBUG 
			printf("后面  往右\r\n");
			#endif
		}
		
		value_old=value;
		return value_return;
	}
	else//如果全白
	{
		#if INFRARED_DEBUG 
		printf("后面  全白\r\n");
		#endif
		return 0;
	}
}

