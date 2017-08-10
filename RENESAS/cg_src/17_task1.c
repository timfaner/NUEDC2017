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
	debug_text("\n run task1\n");
	openmv_error_flag = 0;
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
				openmv_error_handle(&task_continue_flag);
			else
				task_continue_flag = 1;
			//根据错误处理的结果，决定是否继续处理状态
			if(task_continue_flag == 1)
			{
				if(openmv_data[LAND_FLAG] == 1)
				{

						x_offset = rasX_offsetCalculate(openmv_data[SITE_X], PID_HEIGHT);
						y_offset = rasY_offsetCalculate(openmv_data[SITE_Y], PID_HEIGHT);
						//pid
						y_input = y_offset;
						yCompute(&y_input);
						y_speed = y_output;
						x_input = x_offset;
						xCompute(&x_input);
						x_speed = x_output;
						set_new_vel(x_speed, y_speed, LAND_HEIGHT);
						debug_text("Landing status ");
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
						delay_ms(200);

			    }
				else
				{
					follow_car_mode = 1;

					x_offset = rasX_offsetCalculate(openmv_data[SITE_X], PID_HEIGHT);
					y_offset = rasY_offsetCalculate(openmv_data[SITE_Y], PID_HEIGHT);
					//pid
					y_input = y_offset;
					yCompute(&y_input);
					y_speed = y_output;
					x_input = x_offset;
					xCompute(&x_input);
					x_speed = x_output;
					set_new_vel(x_speed, y_speed, TASK_HEIGHT);
					debug_text("Adjusting status ");
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
			debug_text("wait new data\n");
		}
		task_cycle_time_monitor = millis() - task_cycle_timer;
		uart_5_printf("\n\n task1 before preland cycle time %d \n", task_cycle_time_monitor);
	} //task loop
}
