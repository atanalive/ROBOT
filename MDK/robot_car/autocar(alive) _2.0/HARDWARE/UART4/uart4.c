#include "uart4.h"
#include "feedback.h"

//����ѭ�����⴫����
//PC10(TX)  PC11(RX)
void uart4_init(u32 bound)
{
	GPIO_InitTypeDef  gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	//USART2_TX   GPIOC10
	gpio.GPIO_Pin=GPIO_Pin_10;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&gpio);
	
	//USART2_RX	  GPIOC11��ʼ��
	gpio.GPIO_Pin=GPIO_Pin_11;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&gpio);
	
	//NVIC����
	nvic.NVIC_IRQChannel=UART4_IRQn;
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
	usart.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(UART4,&usart);//��ʼ������
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
	USART_Cmd(UART4,ENABLE);//ʹ�ܴ���
}

//(0x0d,0x0a�ǰ��س����͵�)
void UART4_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�
		{
		 res =USART_ReceiveData(UART4);	//��ȡ���յ�������
		 feedback.infrared_front=res;
    } 
}
