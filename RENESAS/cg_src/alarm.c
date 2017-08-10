/*
 * alarm.c
 *
 *  Created on: 2017年8月9日
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
#include "alarm.h"
#include "blink.h"
#include "r_cg_userdefine.h"

void alarm(void)
{
	float distance = 0.0, x_distance = 0.0,
			y_distance = 0.0;

	x_distance = rasX_offsetCalculate(openmv_data[CAR_X], *apm_height);
	y_distance = rasY_offsetCalculate(openmv_data[CAR_Y], *apm_height);
	distance = sqrt( pow(*apm_height, 2) + pow(x_distance, 2) + pow(y_distance, 2));
	uart_5_printf("x_distance: %f  y_distance: %f  height: %f  distance: %f \n",
			x_distance, y_distance, *apm_height, distance);
	if( 0.5 < distance && distance < 1.5)
	{
		distance_alarm();
		distance_blink();
		debug_text('X');
		debug_text("\nsend_to car\n");
		debug_text("alarm\n");
	}
	else
	{
		debug_text('T');
		debug_text("\n send to car\n");
	}
}
