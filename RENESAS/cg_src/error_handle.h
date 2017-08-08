/*
 * error_handle.h
 *
 *  Created on: 2017年8月8日
 *      Author: lenovo
 */

#ifndef SRC_CG_SRC_ERROR_HANDLE_H_
#define SRC_CG_SRC_ERROR_HANDLE_H_

#include "r_cg_macrodriver.h"


void openmv_error_handle(int * _task_continue_flag);
void errorUnknownPlace(void);
void errorCannotGetData(void);
void errorTaskOrder(void);

#endif /* SRC_CG_SRC_ERROR_HANDLE_H_ */
