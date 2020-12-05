#include "tcrt5000.h"

//红外循迹传感器

//PC2,PC3  分别为通道12，13
void tcrt5000_init(void)
{
	//该部分在另一个芯片
	/*
	ADC_InitTypeDef  adc;
	GPIO_InitTypeDef gpio;
	NVIC_InitTypeDef nvic;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置分频因子6，72M/6=12M  ADC最大不能超过14M
	
	gpio.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	gpio.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOC,&gpio);
	
	adc.ADC_Mode=ADC_Mode_Independent;//adc工作模式:ADC1和ADC2工作在独立模式
	adc.ADC_DataAlign=ADC_DataAlign_Right;//数据靠右对齐
	adc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动(不使用外部触发)
	adc.ADC_ContinuousConvMode=ENABLE;//连续转换开启
	adc.ADC_NbrOfChannel=1;//顺序进行规则转换的ADC通道的数目
	adc.ADC_ScanConvMode=DISABLE;//扫描模式关
	ADC_Init(ADC1,&adc);
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1,);
	未写完,待续
	*/
	
}


