#include "led.h"


//PB5 LED0
//PE5 LED1
void LED_Init(void)
{
	GPIO_InitTypeDef gpio;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE,ENABLE);
	
	gpio.GPIO_Pin=GPIO_Pin_5;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	
	GPIO_Init(GPIOE,&gpio);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
	
}



 
