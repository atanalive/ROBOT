#include "vision.h"
#include "fsm.h"
#include "usart.h"

struct VISION_TYPE VISION;
enum VISION_SEND_TYPE VISION_SEND_CONTROL;

/*******关于树莓派返回的值*********/


/*四五位表示数据种类
01表示识别的颜色排列
10表示grab
11表示put
*/

//四五位为10时(grab)
/*低三位
100  小车需左移
001  小车需右移
010  小车位姿调整成功
111  物料抓取成功
000  物料抓取失败
*/

//四五位为11时(put)
/*低三位
100  左移
001  右移
010  小车位姿调整成功
111  物料放置成功
000  物料放置失败
*/


//四五位为01时
//识别从左到右的颜色,并返回三位二进制值(一个字节)（站在物料台外面往里看，从左到右把对应颜色写出来，例如从左到右为红绿蓝：写出来是RGB）
//对应关系如下(从左到右
/*queue_order
001 RBG
010 RGB
011 BRG
100 BGR
101 GRB
110 GBR
*/


/***********计算抓取次序************/
//抽签得到的抓取颜色的次序和视觉得到的颜色的摆放次序并不能直接用
//需要转换成抓取的位置的次序和摆放的位置的次序才能用

//abc acb bac bca cab cba :1~6
//人工算出的抓取次序结果矩阵(第一参为排列次序,第二参为抽签次序)
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
//摆放区域从左到右是红绿蓝:RGB
//计算得到抓取和摆放的位置次序
void calculate_grab_and_put_order(void)
{
	ROBOT_NOW.grab_order=grab_calculate[VISION.queue_order][VISION.draw_order];//得到抓取次序
	switch(VISION.draw_order)//得到摆放次序
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

//对视觉传来的数据进行处理分析
//ROBOT_GRAB=GRAB_SUCCEEDED;
//ROBOT_PUT=PUT_SUCCEEDED;
//协议有所改变，发送的值以协议为准
void analytic_data_in_vision(void)
{
	if(ROBOT_VISION==VISION_START)//发送0x01表示请求树莓派判断物料颜色和位置并返回信息
	{
		switch(VISION_SEND_CONTROL)
		{
			case VISION_ASK_JUDGEMENT_MODE:
				USART_SendData(USART3,0X01);//判断颜色位置模式
			  VISION_SEND_CONTROL=VISION_DO_NOTHING;
			  break;
			case VISION_ASK_GRAB_MODE:
				USART_SendData(USART3,0X02);//小车抓取矫正模式
			  VISION_SEND_CONTROL=VISION_DO_NOTHING;
			  break;
			case VISION_ASK_PUT_MODE:
				USART_SendData(USART3,0X03);//小车投放矫正模式
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
		 case 1://颜色数据
       VISION.queue_order=data;
		   break;
		 case 2://抓取反馈数据
		 	 VISION.grab=data; 
		   break;
	 	 case 3://放置前位置反馈数据
			 VISION.put=data;
		   break;
		 default:
			 printf("VISION is not reserve any data!");
 	 }
  }
}

