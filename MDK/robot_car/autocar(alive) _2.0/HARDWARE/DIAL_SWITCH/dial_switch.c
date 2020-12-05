#include "dial_switch.h"
#include "vision.h"

//PD10~PD15(共六个)
void dial_switch_init(void)
{
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	gpio.GPIO_Mode=GPIO_Mode_IPD;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_ResetBits(GPIOD,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	GPIO_Init(GPIOD,&gpio);
	
}

//则取的顺序为如下:
//1:RGB 2:RBG 3:BRG 4:BGR 5:GRB 6:GBR
u8 search_draw_order(void)
{
	/*
	static u8 i=0;
	while(i!=1)//只能同时一个开关起作用   //判断一次成功后不再进行判断，除非重新上电，不然拨码开关无效，不用每次都进入判断，
	{
	  if(PDin(10)==1){VISION.draw_order=1,i=1;break;}
	  else if(PDin(11)==1){VISION.draw_order=2,i=1;break;}
	  else if(PDin(12)==1){VISION.draw_order=3,i=1;break;}
  	else if(PDin(13)==1){VISION.draw_order=4,i=1;break;}
  	else if(PDin(14)==1){VISION.draw_order=5,i=1;break;}
  	else if(PDin(15)==1){VISION.draw_order=6,i=1;break;}
	}
	*/
	static u8 i=0;//else if 保证了只能一个按钮有效，多个按钮按下的时候，判断顺序优先的奏效，其余无效
	if(PDin(10)==1){VISION.draw_order=1;i=1;}
	else if(PDin(11)==1){VISION.draw_order=2;i=1;}
	else if(PDin(12)==1){VISION.draw_order=3;i=1;}
  else if(PDin(13)==1){VISION.draw_order=4;i=1;}
  else if(PDin(14)==1){VISION.draw_order=5;i=1;}
  else if(PDin(15)==1){VISION.draw_order=6;i=1;}
	else {i=0;}
	return i;
}


