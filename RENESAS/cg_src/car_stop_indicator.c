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
	if(sci5_receive_available())
	{
		SCI5_Serial_Receive(car_cmd, 2);
		if(car_cmd[0] == 1)
			return 1;
		else
		{
			uart_5_printf("error stop flag  received cmd %d\n", car_cmd[0]);
			return 0;
		}
	}
	else
	{
		debug_text("wait for cmd from car\n");
		return 0;
	}
}
