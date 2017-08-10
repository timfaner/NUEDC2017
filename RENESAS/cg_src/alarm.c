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
	static int last_alarm=0,last_last_alarm=0;
	int alarm_flag=0;
	float distance = 0.0, x_distance = 0.0,
			y_distance = 0.0;

	x_distance = rasX_offsetCalculate(openmv_data[CAR_X], *apm_height);
	y_distance = rasY_offsetCalculate(openmv_data[CAR_Y], *apm_height);
	distance = sqrt( pow(*apm_height*100, 2) + pow(x_distance, 2) + pow(y_distance, 2));
	uart_5_printf("x_distance: %f  y_distance: %f  height: %f  distance: %f \n",
			x_distance, y_distance, *apm_height, distance);
	if( 50.0 < distance && distance < 150.0)
	{
		alarm_flag=1;
	}
	else
	{
		alarm_flag=0;
	}
	if(alarm_flag && last_alarm && last_last_alarm){
		SYSTEM_BOOTUP_ALARM = 1;
		BLINK_CONTROL = 1;
		debug_text("X");
		debug_text("alarm\n");
	}
	else if(!alarm_flag && !last_alarm && !last_last_alarm){
		SYSTEM_BOOTUP_ALARM = 0;
		BLINK_CONTROL = 0;
		debug_text("T");
	}
	last_last_alarm=last_alarm;
	last_alarm=alarm_flag;
}
