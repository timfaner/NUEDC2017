/*
 * error_handle.h
 *
 *  Created on: 2017年8月8日
 *      Author: lenovo
 */

#ifndef SRC_CG_SRC_ERROR_HANDLE_H_
#define SRC_CG_SRC_ERROR_HANDLE_H_

#include "r_cg_macrodriver.h"

/************get data from apm*************/
extern float * apm_height;
extern unsigned long last_heartbeat_time;
extern unsigned long runtime;
extern volatile uint8_t openmv_data[7];


void task1_error_handle(int * _task_continue_flag);
void task3_error_handle(int * _task_continue_flag);
void openmv_error_handle(int * _task_continue_flag);
void errorLostGround(void);
void errorLostCar(void);
void errorLostGroundAndCar(void);
void beforeFindCar(void);
void lostGroundBeforeFindCar(void);

#endif /* SRC_CG_SRC_ERROR_HANDLE_H_ */
