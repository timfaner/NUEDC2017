/*
 * 15_task1.c
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
#include "15_task1.h"
#include "r_cg_userdefine.h"

/*********pid parameters********/
extern double x_input, x_output,
		y_input, y_output;

/************get data from apm*************/
extern unsigned long last_heartbeat_time;
extern unsigned long runtime;

/**********timer for tasks*********/
extern unsigned long task_cycle_timer;
extern unsigned long task_cycle_time_monitor;

extern uint8_t last_error_flag;
extern uint16_t flying_count;
extern volatile uint8_t openmv_data[7];
extern volatile uint8_t openmv_error_flag;
extern volatile uint8_t openmv_wrong_order;



void task1(void)
{
	int first_preland_flag = 0;
	int task_continue_flag=1;
	float x_offset = 0.0, y_offset = 0.0,
			x_speed = 0.0, y_speed =0.0;
	float preland_height=0;
	debug_text("\nworking task1\n");
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
			systemEventUpdate(EVENT_SEND_HEARTBEAT);
			debug_text("send heartbeat \n");
		}

		while(openmv_data[DATA_READY] == 0)
		{
			if(millis()-runtime>2000){
				debug_text("OpenMV stop aiding!\n");
				systemErrorUpdate(ERROR_OPENMV_CRASH);
				//TODO: OpenMV Crash Handle
				mav_land();
			}
		}
		if(openmv_data[DATA_READY] == 1)
		{
			if(openmv_error_flag != 0)
				openmv_error_handle(&task_continue_flag);
			//根据错误处理的结果，决定是否继续处理状态
			if(task_continue_flag == 1)
			{
				if(openmv_data[LAND_FLAG] ==1)
				{
					systemEventUpdate(EVENT_LAND);
					while(1)
					{
						mav_land();
						systemEventUpdate(EVENT_LANDED);
						debug_text("\n landed \n");
						delay_ms(200);
					}
			    }
				else
				{
					switch (openmv_data[MAV_STATUS])
					{
						case MAV_STATUS_INIT:
							break;
						case MAV_STATUS_TAKEOFF:
							//dataFushion();
							y_offset = rasY_offsetCalculate(openmv_data[2]);
							//pid
							y_input = y_offset;
							yCompute(&y_input);
							y_speed = y_output;
							//send to apm
							set_new_vel(TASK1_X_SPEED, y_speed, TASK_HEIGHT);
							debug_text("take off status ");
							uart_5_printf(" speed :%f \n", y_speed);
							break;
						case MAV_STATUS_FLYING:
							flying_count++;
							y_offset = rasY_offsetCalculate(openmv_data[2]);
							//pid
							y_input = y_offset;
							yCompute(&y_input);
							y_speed = y_output;

							//send to apm
							set_new_vel(TASK1_X_SPEED, y_speed, TASK_HEIGHT);
							debug_text("flying  ");
							uart_5_printf(" speed :%f \n", y_speed);
							break;
						case MAV_STATUS_PRELAND:
							first_preland_flag++;
							preland_height+=0.005;
							if(TASK_HEIGHT-preland_height<=0.4){
								preland_height=TASK_HEIGHT-0.4;
							}
							//dataFushion();
							x_offset = rasX_offsetCalculate(openmv_data[3]);
							y_offset = rasY_offsetCalculate(openmv_data[2]);
							//pid
							y_input = y_offset;
							yCompute(&y_input);
							y_speed = y_output;

							x_input = x_offset;
							xCompute(&x_input);
							x_speed = x_output;

							//send to apm
							if(first_preland_flag<=20){
								x_speed=0;
								//y_speed=0;
							}
							set_new_vel(x_speed, y_speed, TASK_HEIGHT-preland_height);
							debug_text("preland  ");
							uart_5_printf(" x_speed :%f y_speed %f   height %f \n", x_speed,y_speed,TASK_HEIGHT-preland_height);
							break;
						case MAV_STATUS_OVERFLY:
							if(openmv_wrong_order){
								set_new_vel(TASK1_X_SPEED, 0, TASK_HEIGHT);
							}
							set_new_vel(TASK1_X_SPEED_OVERFLY, 0, TASK_HEIGHT);
							debug_text("over fly \n");
							break;
						case MAV_STATUS_ERROR:
							break;
					}  //choose status
				}  //determine land or not
			}  //determine error flag
			else
			{
				//Skip! error has been handled by openmv_error_handle()
				//taskError();
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
		if(task_cycle_time_monitor > 60)
		{
			uart_5_printf("task timeout: %d\n",task_cycle_time_monitor);
		}
		task_cycle_time_monitor = millis() - task_cycle_timer;
	} //task loop

}
