/*
 * 17_task1.c
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
#include "r_cg_cmt.h"
#include "17_task1.h"
#include "r_cg_userdefine.h"


void task1(void)
{
	int task_continue_flag=1;
	float x_offset = 0.0, y_offset = 0.0,
		  x_speed = 0.0, y_speed =0.0;
	float preland_height=0.0;
	uint8_t cmd = 0;
	uint8_t arm_flag=0;
	uint8_t takeoff_flag=0;
/*************pix & openmv  init****************/
	debug_text("\n run task1\n");
	set_pid(0, 1800.0f, 400.0f, 150.0f, 600.0f);
	set_pid(1, 2000.0f, 300.0f, 200.0f, 400.0f);
	while(!(arm_flag && takeoff_flag))
	{
		pix_init_alarm();
		delay_ms(200);
		if(armcheck())
		{
			arm_flag=1;
		}
		debug_text("arm check!\n");
		if(mav_takeoff(TASK_HEIGHT))
		{
			takeoff_flag=1;
		}
		debug_text("take off!\n");
		if(!(arm_flag && takeoff_flag))
		{
			debug_text("pix init wait\n");
			delay_ms(2000);
		}
	}
	while (*(apm_height) < TASK_HEIGHT-0.2)
	{
		delay_ms(100);
		uart_5_printf("height: %f  wait for Set Height\n",*apm_height);
		if(*apm_height >= LAND_HEIGHT)
		{
			OPENMV_WORK_ENABLE_PIN = 1;
			debug_text("enable openmv\n");
		}
	}
	//set point
	set_new_vel(0.0, 0.0, TASK_HEIGHT);
	while( !(openmv_data[DATA_READY] == 0 || openmv_data[DATA_READY] == 1));  //wait openmv initialize
	openmv_init_alarm();
	set_new_vel(0.0, 0.0, TASK_HEIGHT);
	debug_text("openmv initialized");

	while(1)
	{
		task_continue_flag=1;
		task_cycle_timer = millis();
		runtime = millis();
		if((runtime - last_heartbeat_time) >= 1000)
		{
			//send heartbeat
			S_heartbeat();
			last_heartbeat_time = runtime;
			debug_text("send heartbeat \n");
		}
		while(openmv_data[DATA_READY] == 0)
		{
			if(millis()-runtime>2000)
			{
				debug_text("OpenMV stop aiding!\n");
				//TODO: OpenMV Crash Handle
				mav_land();
			}
		}
		if(openmv_data[DATA_READY] == 1)
		{
			if(openmv_error_flag != 0)
				task1_error_handle(&task_continue_flag);
			else
				task_continue_flag = 1;
			//根据错误处理的结果，决定是否继续处理状态
			if(task_continue_flag == 1)
			{
				if(openmv_data[LAND_FLAG] == 1)
				{
					preland_height += 0.003;
					if(TASK_HEIGHT - preland_height <= LAND_HEIGHT)
					{
						preland_height = TASK_HEIGHT - LAND_HEIGHT;
					}
					x_offset = rasX_offsetCalculate(openmv_data[SITE_Y], *apm_height);
					y_offset = rasY_offsetCalculate(openmv_data[SITE_X], *apm_height);
					//pid
					y_input = y_offset;
					yCompute(&y_input);
					y_speed = y_output;
					x_input = x_offset;
					xCompute(&x_input);
					x_speed = x_output;
					set_new_vel(x_speed, y_speed, TASK_HEIGHT - preland_height);
					debug_text("Landing status ");
					uart_5_printf(" x_offset :%f y_offset %f \n", x_offset,y_offset);
					uart_5_printf(" x_speed :%f y_speed %f   height %f \n", x_speed,y_speed,* apm_height);
					if(* apm_height <= LAND_HEIGHT)
					{
						mav_land();
						debug_text("\n correct height for land \n");
						while(1)
						{
							debug_text("landing... \n");
							delay_ms(200);
						}
					}
			    }
				else
				{

					x_offset = rasX_offsetCalculate(openmv_data[SITE_Y], *apm_height);
					y_offset = rasY_offsetCalculate(openmv_data[SITE_X], *apm_height);
					//pid
					y_input = y_offset;
					yCompute(&y_input);
					y_speed = y_output;
					x_input = x_offset;
					xCompute(&x_input);
					x_speed = x_output;
					set_new_vel(x_speed, y_speed, TASK_HEIGHT);
					debug_text("Adjusting status ");
					uart_5_printf(" x_offset :%f y_offset %f \n", x_offset,y_offset);
					uart_5_printf(" x_speed :%f y_speed %f   height %f \n", x_speed,y_speed,* apm_height);
				}  //choose status
			}
		//update last_error_flag
		last_error_flag = openmv_error_flag;
		//reset data ready flag
		openmv_data[DATA_READY] = 0;
		}  //determine data ready flag
		else
		{
			debug_text("no data\n");
		}
		task_cycle_time_monitor = millis() - task_cycle_timer;
		uart_5_printf("\n\n task1 before preland cycle time %d \n", task_cycle_time_monitor);
	} //task loop
}
