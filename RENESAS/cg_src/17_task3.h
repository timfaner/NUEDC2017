/*
 * 17_task3.h
 *
 *  Created on: 2017年8月9日
 *      Author: lenovo
 */

#ifndef CG_SRC_17_TASK3_H_
#define CG_SRC_17_TASK3_H_

#include "r_cg_macrodriver.h"

/*********pid parameters********/
extern double x_input, x_output,
		y_input, y_output;

/************get data from apm*************/
extern float * apm_height;
extern unsigned long last_heartbeat_time;
extern unsigned long runtime;


extern uint8_t last_error_flag;
extern volatile uint8_t openmv_data[7];
extern volatile uint8_t openmv_error_flag;
extern volatile uint8_t stage_flag;

void task3(void);

#endif /* CG_SRC_17_TASK3_H_ */
