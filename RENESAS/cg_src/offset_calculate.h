/*
 * offset_calculate.h
 *
 *  Created on: 2017年7月26日
 *      Author: lenovo
 */

#ifndef CG_SRC_OFFSET_CALCULATE_H_
#define CG_SRC_OFFSET_CALCULATE_H_

float rasAngleCalculate(float offset);
float rasY_offsetCalculate(float openmv_x_offset, float HEIGHT);
float rasX_offsetCalculate(float openmv_y_offset, float HEIGHT);

#endif /* CG_SRC_OFFSET_CALCULATE_H_ */
