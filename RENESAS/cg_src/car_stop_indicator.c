/*
 * car_stop_indicator.c
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
#include "car_stop_indicator.h"
#include "r_cg_userdefine.h"

int temp_stop_indicator(void)
{
	if(TEMP_STOP_PORT == 1)
	{
		debug_text("receive car stop cmd \n");
		return 1;
	}
	else
	{
		return 0;
	}
}

int stop_indicator(void)
{
	uint8_t data = 0;
	data = car_cmd << 7 & 128;  //1000 0000
	if(data == 128)
	{
		debug_text("receive stop cmd\n");
		return 1;
	}
	else if(data == 0)
	{
		return 0;
	}
	else
	{
		debug_text("error cmd\n");
		return -1;
	}
}
