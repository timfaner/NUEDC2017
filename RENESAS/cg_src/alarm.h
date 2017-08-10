/*
 * alarm.h
 *
 *  Created on: 2017年8月9日
 *      Author: lenovo
 */

#ifndef CG_SRC_ALARM_H_
#define CG_SRC_ALARM_H_

#include "r_cg_macrodriver.h"

extern volatile uint8_t openmv_data[7];

/******get data from apm*******/
extern float * apm_height;

extern uint8_t last_error_flag;
extern volatile uint8_t openmv_data[7];
extern volatile uint8_t openmv_error_flag;
extern volatile uint8_t follow_car_mode;

void alarm(void);

#endif /* CG_SRC_ALARM_H_ */
