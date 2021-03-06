/*
 * car_direction_indicator.h
 *
 *  Created on: 2017年8月10日
 *      Author: lenovo
 */

#ifndef CG_SRC_CAR_DIRECTION_INDICATOR_H_
#define CG_SRC_CAR_DIRECTION_INDICATOR_H_

/************commder from car***********/
extern volatile uint8_t car_cmd;

int temp_direction_indicator(void);
int direction_indicator(void);
float direction_to_x_speed(void);
float direction_to_y_speed(void);

#endif /* CG_SRC_CAR_DIRECTION_INDICATOR_H_ */
