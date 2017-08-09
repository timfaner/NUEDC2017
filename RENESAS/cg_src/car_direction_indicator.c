/*
 * car_direction_indicator.c
 *
 *  Created on: 2017年8月10日
 *      Author: lenovo
 */

/*********imortant*************/
#include "r_cg_macrodriver.h"
/******************************/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "r_cg_sci.h"
//#include "r_cg_cmt.h"
#include "car_direction_indicator.h"
#include "r_cg_userdefine.h"

int temp_direction_indicator(void)
{
	if(TEMP_DIRECTION_PORT1 == 0 && TEMP_DIRECTION_PORT2 == 0)
	{
		debug_text(" fly forward to land \n");
		return FORWARD;
	}
	else if(TEMP_DIRECTION_PORT1 == 0 && TEMP_DIRECTION_PORT2 == 1)
	{
		debug_text(" fly backward to land \n");
		return BACKWARD;
	}
	else if(TEMP_DIRECTION_PORT1 == 1 && TEMP_DIRECTION_PORT2 == 0)
	{
		debug_text(" fly left to land \n");
		return LEFT;
	}
	else if(TEMP_DIRECTION_PORT1 == 1 && TEMP_DIRECTION_PORT2 == 1)
	{
		debug_text(" fly right to land \n");
		return RIGHT;
	}
	else
	{
		debug_text("error direction \n");
		return -1;
	}
}
float direction_to_x_speed(void)
{
	if(temp_direction_indicator == FORWARD)
	{
		return TASK5_FORWARD_X_SPEED;
	}
	else if(temp_direction_indicator == BACKWARD)
	{
		return TASK5_BACKWARD_X_SPEED;
	}
	else if(temp_direction_indicator == LEFT)
	{
		return 0.0;
	}
	else if(temp_direction_indicator == RIGHT)
	{
		return 0.0;
	}
	else
	{
		return 0.0;
	}
}
float direction_to_y_speed(void)
{
	if(temp_direction_indicator == FORWARD)
	{
		return 0.0;
	}
	else if(temp_direction_indicator == BACKWARD)
	{
		return 0.0;
	}
	else if(temp_direction_indicator == LEFT)
	{
		return TASK5_LEFT_Y_SPEED;
	}
	else if(temp_direction_indicator == RIGHT)
	{
		return TASK5_RIGHT_Y_SPEED;
	}
	else
	{
		return 0.0;
	}
}
