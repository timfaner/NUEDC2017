/*
 * 17_task4.c
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
#include "17_task4.h"
#include "r_cg_userdefine.h"
#include "car_stop_indicator.h"

void task4(void)
{
	int task_continue_flag=1;
	int preland_flag = 0;
	int stop_flag = 0;
	unsigned long preland_time = 0;
	unsigned long stop_timer = 0;
	float x_offset = 0.0, y_offset = 0.0,
		  x_speed = 0.0, y_speed =0.0;
	float preland_height = 0.0;
	debug_text("\n run task4\n");
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
				if(temp_stop_indicator() == 1)
				{
						if(stop_flag == 0)
						{
							debug_text("preland timer start\n");
							stop_timer = millis();
						}
						stop_flag = 1;
						runtime = millis();
						if((runtime - last_heartbeat_time) >= 1000)
						{
							//send heartbeat
							S_heartbeat();
							last_heartbeat_time = runtime;
							debug_text("send heartbeat \n");
						}
						if((millis() - stop_timer) >= TASK_DELAY)
						{
							preland_height += 0.003;
							if(GENERAL_HEIGHT - preland_height <= LAND_HEIGHT)
							{
								preland_height = GENERAL_HEIGHT - LAND_HEIGHT;
							}
							if(*apm_height > LAND_HEIGHT)
							{
								x_offset = rasX_offsetCalculate(openmv_data[CAR_X], PID_HEIGHT);
								y_offset = rasY_offsetCalculate(openmv_data[CAR_Y], PID_HEIGHT);
								//pid
								y_input = y_offset;
								yCompute(&y_input);
								y_speed = y_output;
								x_input = x_offset;
								xCompute(&x_input);
								x_speed = x_output;
								set_new_vel(x_speed, y_speed, GENERAL_HEIGHT - preland_height);
								uart_5_printf(" height : %f ", *apm_height);
								debug_text(" time OK, falling... \n");
							}
							else
							{
								set_new_vel(TASK4_X_SPEED, 0.0, LAND_HEIGHT);
								if(preland_flag == 0)
									preland_time = millis();
								preland_flag = 1;
								if((millis() - preland_time) >= LAND_DELAY)
								{
									mav_land();
									while(1)
									{
										debug_text("\n time to land \n");
										delay_ms(200);
									}
								}
								debug_text("reach land height! flying BACKward\n");
							}
						}
						else
						{
							x_offset = rasX_offsetCalculate(openmv_data[CAR_X], PID_HEIGHT);
							y_offset = rasY_offsetCalculate(openmv_data[CAR_Y], PID_HEIGHT);
							//pid
							y_input = y_offset;
							yCompute(&y_input);
							y_speed = y_output;
							x_input = x_offset;
							xCompute(&x_input);
							x_speed = x_output;
							set_new_vel(x_speed, y_speed, TASK_HEIGHT);
							debug_text(" wait for set time \n");
						}
						delay_ms(100);
			    }
				else
				{
					follow_car_mode = 1;

					x_offset = rasX_offsetCalculate(openmv_data[CAR_X], PID_HEIGHT);
					y_offset = rasY_offsetCalculate(openmv_data[CAR_Y], PID_HEIGHT);
					//pid
					y_input = y_offset;
					yCompute(&y_input);
					y_speed = y_output;
					x_input = x_offset;
					xCompute(&x_input);
					x_speed = x_output;
					set_new_vel(x_speed, y_speed, TASK_HEIGHT);
					debug_text("following status ");
					uart_5_printf(" x_speed :%f y_speed %f   height %f \n", x_speed,y_speed,* apm_height);
				}
			}  //determine land or not
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
		uart_5_printf("\n\n task4 before preland cycle time %d \n", task_cycle_time_monitor);
	}
}

