/*
 * 17_task3.c
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
#include "17_task3.h"
#include "r_cg_userdefine.h"

void task3(void)
{
	int task_continue_flag=1;
	unsigned long preland_time = 0;
	int preland_flag = 0;
	float x_offset = 0.0, y_offset = 0.0,
		  x_speed = 0.0, y_speed =0.0;
	debug_text("\n run task3\n");
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
							set_new_vel(x_speed, y_speed, LAND_HEIGHT);
							uart_5_printf("height: %f  falling...\n ", *apm_height);
						}
						else
						{
							set_new_vel(TASK3_X_SPEED, 0.0, LAND_HEIGHT);
							if(preland_flag == 0)
							{
								debug_text("preland timer start\n");
								preland_time = millis();
							}
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
							debug_text("reach land height! flying forward\n");
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
		uart_5_printf("\n\n task3 before preland cycle time %d \n", task_cycle_time_monitor);
	}
}

