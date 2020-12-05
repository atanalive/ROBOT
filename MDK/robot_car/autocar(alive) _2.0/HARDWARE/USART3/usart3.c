#include "usart3.h"
#include "vision.h"
#include "fsm.h"

//������ݮ������(���ڲ���������ʱ����Ϊ115200)
//PB10  PB11
void usart3_init(u32 bound)
{
	GPIO_InitTypeDef  gpio;
	USART_InitTypeDef usart3;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//USART3_TX   GPIOA.9
	gpio.GPIO_Pin=GPIO_Pin_10;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio);
	
	//USART1_RX	  GPIOA.10��ʼ��
	gpio.GPIO_Pin=GPIO_Pin_11;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&gpio);
	
	//NVIC����
	nvic.NVIC_IRQChannel=USART3_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=3;
	nvic.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&nvic);
	
	//usart��ʼ��
	usart3.USART_BaudRate=bound;//���ڲ�����
	usart3.USART_WordLength=USART_WordLength_8b;//�ֽ�Ϊ8λ���ݸ�ʽ
	usart3.USART_Parity=USART_Parity_No;//����żУ��λ
	usart3.USART_StopBits=USART_StopBits_1;//һ��ֹͣλ
	//ֹͣλ��Ҫ��֪ͨ�����豸����֡������ɣ����û��ֹͣλ�������豸��һֱ���ڽ���״̬��û�취������һ֡����
	usart3.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ������������
	//����Ӳ������������һ�����ڰ�˫��ʱ���շ��л���RTS:�ⲿ�����ͱ�־λ/CTS�����豸�Ƿ�����ܷ���ձ�־λ����ֹδ׼���þͷ�������������ݶ�ʧ��
	usart3.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�շ�ģʽ
	
	USART_Init(USART3,&usart3);//��ʼ������
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3,ENABLE);//ʹ�ܴ���
}

//(0x0d,0x0a�ǰ��س����͵�)
void USART3_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
		{
		 res =USART_ReceiveData(USART3);	//��ȡ���յ�������
		 if(ROBOT_VISION==VISION_START)
     {
			 VISION.receive=res;
		 }
    } 
}



