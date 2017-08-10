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
#include "r_cg_cmt.h"
#include "error_handle.h"
#include "r_cg_userdefine.h"


//extern uint8_t last_error_flag;
extern volatile uint8_t openmv_error_flag;
extern uint8_t last_error_flag;
volatile uint8_t follow_car_mode = 0;

void task1_error_handle(int * _task_continue_flag)
{
	static uint8_t error_count = 0;
	if(openmv_error_flag == 1 || openmv_error_flag == 3)
	{
		if(last_error_flag == 1 || last_error_flag == 3)
			error_count++;
		else
			error_count = 0;
//		debug_text(" lost ground!\n");
//		* _task_continue_flag = 0;
		if(openmv_error_flag == 3)
			debug_text("error flag = 3\n");
		else
			debug_text("error flag = 1\n");
		if(error_count>=30)
			errorLostGround();
		else
			* _task_continue_flag = 0;
	}
	else
	{
		* _task_continue_flag = 1;
	}
}

void task3_error_handle(int * _task_continue_flag)
{

}

void openmv_error_handle(int * _task_continue_flag)
{
	if(openmv_error_flag == 1 && follow_car_mode == 1)
	{
		if(last_error_flag == 1)
		{
			*_task_continue_flag= 1;
		}
		else
			errorLostGround();
	}
	else if(openmv_error_flag == 1 && follow_car_mode == 0)
	{
		debug_text("find the car \n");
		*_task_continue_flag= 1;
	}
	else if(openmv_error_flag == 2 && follow_car_mode == 1)
	{
		errorLostCar();
	}
	else if(openmv_error_flag == 2 && follow_car_mode == 0)
	{
		beforeFindCar();
		*_task_continue_flag=0;
//		openmv_error_flag=0;
	}
	else if(openmv_error_flag ==3 && follow_car_mode == 0)
	{
		lostGroundBeforeFindCar();
		*_task_continue_flag=0;
//		openmv_error_flag=0;
	}
	else if(openmv_error_flag == 3 && follow_car_mode ==1)
	{
		errorLostGroundAndCar();
	}

}

void errorLostGround(void)
{
	mav_land();
	while(1)
	{
		debug_text(" lost ground! land !\n");
		delay_ms(200);
	}
}
void errorLostCar(void)
{
	mav_land();
	while(1)
	{
		debug_text(" lost car! land !\n");
		delay_ms(200);
	}
}
void errorLostGroundAndCar(void)
{
	mav_land();
	while(1)
	{
		debug_text(" lost ground and car!! land !\n");
		delay_ms(200);
	}
}
void beforeFindCar(void)
{
	set_new_vel(TASK3_X_SPEED, 0.0, TASK_HEIGHT);
	debug_text("have not found car\n");
}
void lostGroundBeforeFindCar(void)
{
	set_new_vel(TASK3_X_SPEED, 0.0, TASK_HEIGHT);
	debug_text("lost line before find car\n");
}
