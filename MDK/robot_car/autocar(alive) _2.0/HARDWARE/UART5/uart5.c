#include "uart5.h"
#include "feedback.h"

//����ѭ�����⴫����
//PC12(TX)  PD2(RX)
void uart5_init(u32 bound)
{
	GPIO_InitTypeDef  gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	
	//USART2_TX ���  GPIOC12
	gpio.GPIO_Pin=GPIO_Pin_12;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	//USART2_RX	����  GPIOD2��ʼ��
	gpio.GPIO_Pin=GPIO_Pin_2;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&gpio);
	
	//NVIC����
	nvic.NVIC_IRQChannel=UART5_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=2;
	nvic.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&nvic);
	
	//usart��ʼ��
	usart.USART_BaudRate=bound;//���ڲ�����
	usart.USART_WordLength=USART_WordLength_8b;//�ֽ�Ϊ8λ���ݸ�ʽ
	usart.USART_Parity=USART_Parity_No;//����żУ��λ
	usart.USART_StopBits=USART_StopBits_1;//һ��ֹͣλ
	//ֹͣλ��Ҫ��֪ͨ�����豸����֡������ɣ����û��ֹͣλ�������豸��һֱ���ڽ���״̬��û�취������һ֡����
	usart.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ������������
	//����Ӳ������������һ�����ڰ�˫��ʱ���շ��л���RTS:�ⲿ�����ͱ�־λ/CTS�����豸�Ƿ�����ܷ���ձ�־λ����ֹδ׼���þͷ�������������ݶ�ʧ��
	//usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
	usart.USART_Mode=USART_Mode_Rx;//����ģʽ
	
	USART_Init(UART5,&usart);//��ʼ������
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
	USART_Cmd(UART5,ENABLE);//ʹ�ܴ���
}

//(0x0d,0x0a�ǰ��س����͵�)
void UART5_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�
		{
		 res =USART_ReceiveData(UART5);	//��ȡ���յ�������
		 feedback.infrared_right=res;
    } 
}
