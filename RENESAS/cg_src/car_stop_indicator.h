/*
 * car_stop_indicator.h
 *
 *  Created on: 2017年8月10日
 *      Author: lenovo
 */

#ifndef CG_SRC_CAR_STOP_INDICATOR_H_
#define CG_SRC_CAR_STOP_INDICATOR_H_

/************commder from car***********/
extern volatile uint8_t car_cmd;

int stop_indicator(void);
int temp_stop_indicator(void);

#endif /* CG_SRC_CAR_STOP_INDICATOR_H_ */
