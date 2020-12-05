#include "path.h"
#include "fsm.h"
#include "usart.h"
#include "feedback.h"

PATH_type PATH_NOW={calibration,path_left,path_left};

/******************路径的选择*****************/
//选择路径
void choice_path(void)
{
	switch(ROBOT_STATE)
	{
		case STANDBY:
			PATH_NOW.now_path=stanby;
		  break;
		case CALIBRATION:
			PATH_NOW.now_path=calibration;
		  break;
		case START_TO_MIDDLE:
			PATH_NOW.now_path=start_to_middle;
		  PATH_NOW.now_location=path_middle;
			break;
		case MIDDLE_TO_GRAB:
			PATH_NOW.now_path=middle_to_grab;
		  switch(VISION.grab)
			{
				case 1:
				case 2:
					PATH_NOW.next_location=path_left;
					break;
				case 3:
				case 4:
					PATH_NOW.next_location=path_middle;
					break;
				case 5:
				case 6:
					PATH_NOW.next_location=path_right;
					break;
			}
			break;
		case VISION_JUDGEMENT:
		case GRAB:
			PATH_NOW.now_path=stanby;
		  break;		
		case GRAB_TO_PUT:
			PATH_NOW.now_path=grab_to_put;
			switch(STATE_COUNT.TURNAROUND_TO_PUT_count)
			{
				case 1:
					switch(ROBOT_NOW.put_order)
					{
						case 1:
						case 2:
							PATH_NOW.next_location=path_left;
							break;
						case 3:
						case 4:
							PATH_NOW.next_location=path_middle;
						  break;
						case 5:
						case 6:
							PATH_NOW.next_location=path_right;
						  break;
					}
				case 2:
					switch(ROBOT_NOW.put_order)
					{
						case 3:
						case 5:
							PATH_NOW.next_location=path_left;
							break;
						case 1:
						case 6:
							PATH_NOW.next_location=path_middle;
						  break;
						case 2:
						case 4:
							PATH_NOW.next_location=path_right;
						  break;
					}
				case 3:
					switch(ROBOT_NOW.put_order)
					{
						case 4:
						case 6:
							PATH_NOW.next_location=path_left;
							break;
						case 2:
						case 5:
							PATH_NOW.next_location=path_middle;
						  break;
						case 1:
						case 3:
							PATH_NOW.next_location=path_right;
						  break;
					}
			}
			break;
		case PUT:
			PATH_NOW.now_path=stanby;
			break;
		case RETURN_TO_GRAB:
			PATH_NOW.now_path=return_to_grab;
			switch(STATE_COUNT.TURNAROUND_TO_TABLE_count)
			{
				case 1:
					switch(ROBOT_NOW.grab_order)
					{
						case 3:
						case 5:
							PATH_NOW.next_location=path_left;
							break;
						case 1:
						case 6:
							PATH_NOW.next_location=path_middle;
						  break;
						case 2:
						case 4:
							PATH_NOW.next_location=path_right;
						  break;
					}
					break;
				case 2:
					switch(ROBOT_NOW.grab_order)
					{
						case 4:
						case 6:
							PATH_NOW.next_location=path_left;
							break;
						case 2:
						case 5:
							PATH_NOW.next_location=path_middle;
							break;
						case 1:
						case 3:
							PATH_NOW.next_location=path_right;
							break;
					}
					break;
			}
			break;
		case RETURN_TO_START:
			PATH_NOW.now_path=return_to_start;
			break;
	}
	
}	




