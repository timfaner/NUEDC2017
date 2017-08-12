/*
 * 17_task2.c
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
#include "17_task2.h"
#include "r_cg_userdefine.h"

void task2(void)
{
	OPENMV_WORK_ENABLE_PIN = 1; //通知openmv开始工作 将该引脚置高
	while( !(openmv_data[DATA_READY] == 0 || openmv_data[DATA_READY] == 1));  //wait openmv initialize
	openmv_init_alarm();
//	debug_text("openmv initialized");
//	debug_text("\n run task2\n");
	while(1)
	{
		runtime = millis();
		if((runtime - last_heartbeat_time) >= 1000)
		{
			//send heartbeat
			S_heartbeat();
			last_heartbeat_time = runtime;
//			debug_text("send heartbeat \n");
		}
		while(openmv_data[DATA_READY] == 0)
		{
			if(millis()-runtime>2000)
			{
//				debug_text("OpenMV stop aiding!\n");
				SYSTEM_BOOTUP_ALARM = 0;
				BLINK_CONTROL = 0;
				debug_text("T");
				//TODO: OpenMV Crash Handle
				mav_land();
			}
		}
		if(openmv_data[DATA_READY] == 1)
		{
			if(openmv_error_flag == 2)
			{
				debug_text("T");
				SYSTEM_BOOTUP_ALARM = 0;
				BLINK_CONTROL = 0;
			}
			else if(openmv_error_flag != 3)
			{
				alarm();
			}
			else
			{
//				debug_text("other error\n");
				SYSTEM_BOOTUP_ALARM = 0;
				BLINK_CONTROL = 0;
				debug_text("T");
			}
			openmv_data[DATA_READY] = 0;
		}
//		uart_5_printf("cycle time %d\n", millis() - runtime);
	}
}
