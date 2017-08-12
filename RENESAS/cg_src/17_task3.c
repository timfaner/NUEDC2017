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


extern float read_height(void);

void task3(void)
{
	int task_continue_flag=1;
	unsigned long preland_time = 0;
	int preland_flag = 0;
	float x_offset = 0.0, y_offset = 0.0,
		  x_speed = 0.0, y_speed =0.0;
	float preland_height = 0.0;

	uint8_t stage_count = 0;
	uint8_t steady_count = 0;
	uint8_t last_stage_flag = 0;

	uint8_t arm_flag=0;
	uint8_t takeoff_flag=0;
/*************pix & openmv  init****************/
	debug_text("\n run task3\n");
	set_pid(0, 3600.0f, 200.0f, 300.0f, 450.0f);
	set_pid(1, 3200.0f, 180.0f, 250.0f, 400.0f);
	while(!(arm_flag && takeoff_flag))
	{
		pix_init_alarm();
		delay_ms(200);
		if(armcheck())
		{
			arm_flag=1;
		}
		debug_text("arm check!\n");
		if(mav_takeoff(GENERAL_HEIGHT))
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
	//set point
	while((read_height()) <= 0.2);
	set_new_vel(0.0f,0.0f,GENERAL_HEIGHT);
	while((read_height()) <= LAND_HEIGHT);
	OPENMV_WORK_ENABLE_PIN = 1;
	debug_text("enable openmv\n");
	//set point
	set_new_vel(0.0f, 0.0f, GENERAL_HEIGHT);
	while( !(openmv_data[DATA_READY] == 0 || openmv_data[DATA_READY] == 1));  //wait openmv initialize
	openmv_init_alarm();
	set_new_vel(0.0, 0.0, GENERAL_HEIGHT);
	debug_text("openmv initialized");

	while(1)
	{
		task_continue_flag=1;
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
//				uart_5_printf("ready flag  %d ", openmv_data[DATA_READY]);
				//TODO: OpenMV Crash Handle
				set_new_vel(0.0f, 0.0f, 0.2f);
				mav_land();
			}
		}
		if(openmv_data[DATA_READY] == 1)
		{
			uart_5_printf("ZT %d %d %d %d %d %d\n", openmv_data[ERROR_FLAG],openmv_data[LAND_FLAG],
						openmv_data[SITE_X],openmv_data[SITE_Y], openmv_data[CAR_X],openmv_data[CAR_Y]);
			uart_5_printf("land flag %d ", openmv_data[LAND_FLAG]);
			uart_5_printf("stage_flag %d   error_flag %d\n\n", stage_flag, openmv_error_flag);
			if(openmv_error_flag != 0)
				task3_error_handle(&task_continue_flag);
			else
				task_continue_flag = 1;
			//根据错误处理的结果，决定是否继续处理状态
			if(task_continue_flag == 1)
			{
				if(openmv_data[LAND_FLAG] == 1)
				{
					preland_height += 0.003;
					if(GENERAL_HEIGHT - preland_height <= LAND_HEIGHT)
					{
						preland_height = GENERAL_HEIGHT - LAND_HEIGHT;
					}
					if(read_height() > LAND_HEIGHT)
					{
						x_offset = rasX_offsetCalculate(openmv_data[CAR_Y], read_height());
						y_offset = rasY_offsetCalculate(openmv_data[CAR_X], read_height());
						//pid
						y_input = y_offset;
						yCompute(&y_input);
						y_speed = y_output;
						x_input = x_offset;
						xCompute(&x_input);
						x_speed = x_output;
						set_new_vel(x_speed, y_speed, GENERAL_HEIGHT - preland_height);
						uart_5_printf("height: %f  falling...\n ", read_height());
					}
					else
					{
						set_new_vel(-TASK3_X_SPEED, 0.0, LAND_HEIGHT);
						if(preland_flag == 0)
						{
							debug_text("preland timer start\n");
							preland_time = millis();
						}
						preland_flag = 1;
						if((millis() - preland_time) >= LAND_DELAY)
						{
							set_new_vel(0.0, 0.0, 0.2);
							mav_land();
							while(1)
							{
								debug_text("\n landing... \n");
								delay_ms(200);
							}
						}
						debug_text("reach land height! flying forward\n");
					}
				}
				else
				{
					if(stage_flag == SET_POINT)
					{
						x_offset = rasX_offsetCalculate(openmv_data[SITE_Y], read_height());
						y_offset = rasY_offsetCalculate(openmv_data[SITE_X], read_height());
						//pid
						y_input = y_offset;
						yCompute(&y_input);
						y_speed = y_output;
						x_input = x_offset;
						xCompute(&x_input);
						x_speed = x_output;
						set_new_vel(x_speed, y_speed, GENERAL_HEIGHT);
						debug_text("hold on status \n");
						uart_5_printf("XO %f %f", x_offset,y_offset);
						uart_5_printf(" %f %f %f \n", x_speed,y_speed, read_height());

						stage_count++;
						if(stage_count >= 60)
						{
							stage_flag = FINDING_CAR;
						}
					}
					else if(stage_flag == FINDING_CAR)
					{
						if(openmv_error_flag == 2)
						{
							y_offset = rasY_offsetCalculate(openmv_data[SITE_X], read_height());
							//pid
							y_input = y_offset;
							yCompute(&y_input);
							y_speed = y_output;
							x_speed = TASK3_X_SPEED;
							set_new_vel(TASK3_X_SPEED, y_speed, GENERAL_HEIGHT);
							debug_text("finding status,To stage 1 \n");
							uart_5_printf("XO %f %f", x_offset,y_offset);
							uart_5_printf(" %f %f %f \n", x_speed,y_speed, read_height());
						}
						else if(openmv_error_flag == 3){
							set_new_vel(TASK3_X_SPEED, 0.0, GENERAL_HEIGHT);
						}
						else{
							stage_flag == FOLLOWING_CAR;

							x_offset = rasX_offsetCalculate(openmv_data[CAR_Y], read_height());
							y_offset = rasY_offsetCalculate(openmv_data[CAR_X], read_height());
							//pid
							y_input = y_offset;
							yCompute(&y_input);
							y_speed = y_output;
							x_input = x_offset;
							xCompute(&x_input);
							x_speed = x_output;
							set_new_vel(x_speed, y_speed, GENERAL_HEIGHT);
							debug_text("following status \n");
							uart_5_printf("XO %f %f", x_offset,y_offset);
							uart_5_printf(" %f %f %f \n", x_speed,y_speed, read_height());
						}

					}
					else if(stage_flag == FOLLOWING_CAR)
					{
						if(last_stage_flag == FOLLOWING_CAR)
						{
							steady_count++;
						}
						else
						{
							steady_count = 1;
						}
						if(steady_count <= 10)
						{
							set_new_vel(0.0f, 0.0f, read_height());
						}
						else
						{
							if(openmv_error_flag == 2)
							{
								y_offset = rasY_offsetCalculate(openmv_data[SITE_X], read_height());
								//pid
								y_input = y_offset;
								yCompute(&y_input);
								y_speed = y_output;
								x_speed = TASK3_X_SPEED;
								set_new_vel(TASK3_X_SPEED, y_speed, GENERAL_HEIGHT);
								debug_text("lost car,see site, refinding status \n");
								uart_5_printf("XO %f %f", x_offset,y_offset);
								uart_5_printf(" %f %f %f \n", x_speed,y_speed, read_height());

								stage_flag = FINDING_CAR;
							}
							else if(openmv_error_flag == 0 || openmv_error_flag == 1)
							{
								x_offset = rasX_offsetCalculate(openmv_data[CAR_Y], read_height());
								y_offset = rasY_offsetCalculate(openmv_data[CAR_X], read_height());
								//pid
								y_input = y_offset;
								yCompute(&y_input);
								y_speed = y_output;
								x_input = x_offset;
								xCompute(&x_input);
								x_speed = x_output;
								set_new_vel(x_speed, y_speed, GENERAL_HEIGHT);
								debug_text("following status \n");
								uart_5_printf("XO %f %f", x_offset,y_offset);
								uart_5_printf(" %f %f %f \n", x_speed,y_speed, read_height());;
							}
						}
					}
					last_stage_flag = stage_flag;
				}
			}  //determine land or not
		//update last_error_flag
		last_error_flag = openmv_error_flag;
		//reset data ready flag
		openmv_data[DATA_READY] = 0;
		}  //determine data ready flag
		else
		{
			debug_text("no data\n");
		}
	}
}

