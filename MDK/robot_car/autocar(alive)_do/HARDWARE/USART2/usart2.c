#include "usart2.h"
#include "feedback.h"

//���ڴ���������
//PA2(TX)  PA3(RX)
void usart2_init(u32 bound)
{
	GPIO_InitTypeDef  gpio;
	USART_InitTypeDef usart;
	NVIC_InitTypeDef nvic;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//USART2_TX   GPIOA2
	gpio.GPIO_Pin=GPIO_Pin_2;
	gpio.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio);
	
	//USART2_RX	  GPIOA3��ʼ��
	gpio.GPIO_Pin=GPIO_Pin_3;
	gpio.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&gpio);
	
	//NVIC����
	nvic.NVIC_IRQChannel=USART2_IRQn;
	nvic.NVIC_IRQChannelCmd=ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority=3;
	nvic.NVIC_IRQChannelSubPriority=3;
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
	
	USART_Init(USART2,&usart);//��ʼ������
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�������ڽ����ж�
	USART_Cmd(USART2,ENABLE);//ʹ�ܴ���
}

//(0x0d,0x0a�ǰ��س����͵�)
void USART2_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
		{
		 res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		 feedback.mpu6050_info=res;
    } 
}

