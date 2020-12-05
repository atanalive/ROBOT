#include "vision.h"
#include "fsm.h"
#include "usart.h"

struct VISION_TYPE VISION;
enum VISION_SEND_TYPE VISION_SEND_CONTROL;

/*******������ݮ�ɷ��ص�ֵ*********/


/*����λ��ʾ��������
01��ʾʶ�����ɫ����
10��ʾgrab
11��ʾput
*/

//����λΪ10ʱ(grab)
/*����λ
100  С��������
001  С��������
010  С��λ�˵����ɹ�
111  ����ץȡ�ɹ�
000  ����ץȡʧ��
*/

//����λΪ11ʱ(put)
/*����λ
100  ����
001  ����
010  С��λ�˵����ɹ�
111  ���Ϸ��óɹ�
000  ���Ϸ���ʧ��
*/


//����λΪ01ʱ
//ʶ������ҵ���ɫ,��������λ������ֵ(һ���ֽ�)��վ������̨�������￴�������ҰѶ�Ӧ��ɫд���������������Ϊ��������д������RGB��
//��Ӧ��ϵ����(������
/*queue_order
001 RBG
010 RGB
011 BRG
100 BGR
101 GRB
110 GBR
*/


/***********����ץȡ����************/
//��ǩ�õ���ץȡ��ɫ�Ĵ�����Ӿ��õ�����ɫ�İڷŴ��򲢲���ֱ����
//��Ҫת����ץȡ��λ�õĴ���Ͱڷŵ�λ�õĴ��������

//abc acb bac bca cab cba :1~6
//�˹������ץȡ����������(��һ��Ϊ���д���,�ڶ���Ϊ��ǩ����)
u8 grab_calculate[6][6]=
{
	{1,2,3,4,5,6},
	{2,1,5,6,3,4},
	{3,4,1,2,6,4},
	{4,6,2,1,4,3},
	{5,3,6,5,1,2},
	{6,4,4,3,2,1},
};

//
//�ڷ�����������Ǻ�����:RGB
//����õ�ץȡ�Ͱڷŵ�λ�ô���
void calculate_grab_and_put_order(void)
{
	ROBOT_NOW.grab_order=grab_calculate[VISION.queue_order][VISION.draw_order];//�õ�ץȡ����
	switch(VISION.draw_order)//�õ��ڷŴ���
  {
		case 1:
			ROBOT_NOW.put_order=2;
		  break;
		case 2:
			ROBOT_NOW.put_order=1;
		  break;
		case 3:
			ROBOT_NOW.put_order=5;
		  break;
		case 4:
			ROBOT_NOW.put_order=6;
		  break;
		case 5:
			ROBOT_NOW.put_order=3;
		  break;
		case 6:
			ROBOT_NOW.put_order=4;
		  break;
	}
}

//���Ӿ����������ݽ��д������
//ROBOT_GRAB=GRAB_SUCCEEDED;
//ROBOT_PUT=PUT_SUCCEEDED;
//Э�������ı䣬���͵�ֵ��Э��Ϊ׼
void analytic_data_in_vision(void)
{
	if(ROBOT_VISION==VISION_START)//����0x01��ʾ������ݮ���ж�������ɫ��λ�ò�������Ϣ
	{
		switch(VISION_SEND_CONTROL)
		{
			case VISION_ASK_JUDGEMENT_MODE:
				USART_SendData(USART3,0X01);//�ж���ɫλ��ģʽ
			  VISION_SEND_CONTROL=VISION_DO_NOTHING;
			  break;
			case VISION_ASK_GRAB_MODE:
				USART_SendData(USART3,0X02);//С��ץȡ����ģʽ
			  VISION_SEND_CONTROL=VISION_DO_NOTHING;
			  break;
			case VISION_ASK_PUT_MODE:
				USART_SendData(USART3,0X03);//С��Ͷ�Ž���ģʽ
			  VISION_SEND_CONTROL=VISION_DO_NOTHING;
			  break;
			case VISION_DO_NOTHING:
				break;
		}
	}
	if((VISION.receive>>3)!=0)
	{
	 u8 data=VISION.receive&0x07;
	 switch(VISION.receive>>3)
	 {
		 case 1://��ɫ����
       VISION.queue_order=data;
		   break;
		 case 2://ץȡ��������
		 	 VISION.grab=data; 
		   break;
	 	 case 3://����ǰλ�÷�������
			 VISION.put=data;
		   break;
		 default:
			 printf("VISION is not reserve any data!");
 	 }
  }
}

