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

extern float read_height(void);

//extern uint8_t last_error_flag;
extern volatile uint8_t openmv_error_flag;
extern uint8_t last_error_flag;
volatile uint8_t follow_car_mode = 0;
volatile uint8_t stage_flag = 0;

void task1_error_handle(int * _task_continue_flag)
{
	static uint8_t error_count = 0;
	if(openmv_error_flag == 1 || openmv_error_flag == 3)
	{
		if(last_error_flag == 1 || last_error_flag == 3)
			error_count++;
		else
			error_count = 0;
		if(openmv_error_flag == 3)
			debug_text("error flag = 3\n");
		else
			debug_text("error flag = 1\n");
		if(error_count >= 10)
			set_new_vel(0.0f, 0.0f, read_height());
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
	static uint8_t error_count = 0;
	static uint8_t error_count2 = 0;
	static uint8_t error_count3 = 0;
	if(openmv_error_flag == 1)
	{
		if(last_error_flag == 1)
			error_count3++;
		else
			error_count3 = 1;
		* _task_continue_flag = 1;
		if(error_count3 >= 5)
		{
			debug_text("find car\n");
			stage_flag = FOLLOWING_CAR;
		}
	}
	if(stage_flag == SET_POINT && openmv_error_flag != 1)
	{
		if(openmv_error_flag == 2)
		{
			* _task_continue_flag = 1;
		}
		else
		{
			if(last_error_flag == 3)
				error_count++;
			else
				error_count = 1;
			if(error_count >= 30)
			{
				errorLostGroundAndCar();
			}
			else
			{
				debug_text("error flag = 3\n");
			}
		}
	}
	else if(stage_flag == FINDING_CAR && openmv_error_flag != 1)
	{
		* _task_continue_flag = 1;
	}
	else if(stage_flag == FOLLOWING_CAR && openmv_error_flag != 1)
	{
		if(openmv_error_flag == 2)
		{
			debug_text("lost car, use ground \n");
			* _task_continue_flag = 1;
		}
		else
		{
			if(last_error_flag == 3)
				error_count2++;
			else
				error_count2 = 1;
			if(error_count2 >= 30)
			{
				errorLostGroundAndCar();
			}
			else
			{
				debug_text("error flag = 3\n");
				set_new_vel(0.0f, 0.0f, read_height());
			}
		}
	}
	//else data rev error
}


void errorLostGround(void)
{
	set_new_vel(0.0f, 0.0f, 0.2f);
	debug_text("lost ground ");
	uart_5_printf("height: %f  falling...\n ", read_height());
	mav_land();
	while(1)
	{
		debug_text(" lost ground! land !\n");
		delay_ms(200);
	}
}
void errorLostCar(void)
{
	set_new_vel(0.0f, 0.0f, 0.2f);
	debug_text("lost car ");
	uart_5_printf("height: %f  falling...\n ", read_height());
	set_new_vel(0.0f, 0.0f, 0.2f);
	mav_land();
	while(1)
	{
		debug_text(" lost car! land !\n");
		delay_ms(200);
	}
}
void errorLostGroundAndCar(void)
{
	set_new_vel(0.0f, 0.0f, 0.2f);
	debug_text("lost car and ground ");
	uart_5_printf("height: %f  falling...\n ", read_height());
	set_new_vel(0.0f, 0.0f, 0.2f);
	mav_land();
	while(1)
	{
		debug_text("  land... !\n");
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
		if(last_error_flag == 1 )
		{
			debug_text("lost car and find site\n");
			follow_car_mode = 0;
		}
		else
		{
			errorLostCar();
		}
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
