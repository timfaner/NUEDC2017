/*
 * error_handle.c
 *
 *  Created on: 2017年8月8日
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
#include "error_handle.h"
#include "r_cg_userdefine.h"


extern uint8_t last_error_flag;
extern uint16_t flying_count;
extern volatile uint8_t openmv_data[7];
extern volatile uint8_t openmv_error_flag;
extern volatile uint8_t openmv_wrong_order;

void openmv_error_handle(int * _task_continue_flag)
{
	static int error_count=1;
	static int no_color_count=0;
	if(last_error_flag != 0)
	{
		error_count++;
	}
	else{
		error_count=1;
		//no_color_count=0;
	}
	uart_5_printf("[taskError()] Task error  %d\n" ,openmv_error_flag);
//	systemErrorUpdate(ERROR_SPI_DATA);
	if(error_count >= TASK_ERROR_THRESHOLD)
	{
		mav_land();
		debug_text("\n[taskError()] Error overflow! forced landing \n");
//		systemErrorUpdate(ERROR_OPENMV_DATA_ABNORMAL);
	}
	if(openmv_error_flag == 1)
	{
		errorUnknownPlace();
		*_task_continue_flag=0;
		openmv_error_flag=0;
	}
	else if(openmv_error_flag == 2)
	{
		no_color_count++;
		*_task_continue_flag=0;
		if(no_color_count>=20){
			errorCannotGetData();
		}
		openmv_error_flag=0;
	}
	else if(openmv_error_flag == 3)
	{
		openmv_wrong_order = 1;
		if(flying_count>=50){
			*_task_continue_flag=1;
		}
		else{
			*_task_continue_flag=0;
			errorTaskOrder();
		}

	}
}

void errorUnknownPlace(void)
{
	debug_text("unknown place\n");
	set_new_vel(0, 0, TASK_ERROR_HEIGHT);
}
void errorCannotGetData(void)
{
	debug_text("error can not get color\n");
	mav_land();
	while(1)
	{
		debug_text(" can not get color! land !\n");
		delay_ms(200);
	}
}
void errorTaskOrder(void)
{
	if(openmv_data[MAV_STATUS] ==MAV_STATUS_FLYING){
		openmv_error_flag=0;
	}
	else{
		debug_text("wrong order, Fliyng Forward\n");
		set_new_vel(TASK1_X_SPEED, 0, TASK_HEIGHT);
	}
}
