#include "tcrt5000.h"

//����ѭ��������

//PC2,PC3  �ֱ�Ϊͨ��12��13
void tcrt5000_init(void)
{
	//�ò�������һ��оƬ
	/*
	ADC_InitTypeDef  adc;
	GPIO_InitTypeDef gpio;
	NVIC_InitTypeDef nvic;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//���÷�Ƶ����6��72M/6=12M  ADC����ܳ���14M
	
	gpio.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	gpio.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOC,&gpio);
	
	adc.ADC_Mode=ADC_Mode_Independent;//adc����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	adc.ADC_DataAlign=ADC_DataAlign_Right;//���ݿ��Ҷ���
	adc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//ת��������������ⲿ��������(��ʹ���ⲿ����)
	adc.ADC_ContinuousConvMode=ENABLE;//����ת������
	adc.ADC_NbrOfChannel=1;//˳����й���ת����ADCͨ������Ŀ
	adc.ADC_ScanConvMode=DISABLE;//ɨ��ģʽ��
	ADC_Init(ADC1,&adc);
	
	ADC_Cmd(ADC1,ENABLE);
	ADC_RegularChannelConfig(ADC1,);
	δд��,����
	*/
	
}


