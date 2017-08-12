/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : Code Generator for RX23T V1.00.01.01 [30 Oct 2015]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file includes user definition.
* Creation Date: 2017/7/17
***********************************************************************************************************************/
#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/
#define FAST_INTERRUPT_VECTOR 0

/* Start user code for function. Do not edit comment generated here */
#include "../Mavlink_lib/Mavlink_lib.h"
#include "pid.h"
#include "blink.h"
#include "buzzer.h"
#include "alarm.h"
#include "17_task1.h"
#include "user_main.h"
#include "error_handle.h"
#include "task_choose.h"
#include "systemmonitor.h"
#include "offset_calculate.h"

/***************port define*****************/

#define RSA_WORK_ENABLE_PIN     PORT3.PIDR.BIT.B0   //NO.45
#define RSA_TASK_SWICH1         PORT7.PIDR.BIT.B0   //NO.39
#define RSA_TASK_SWICH2         PORT3.PIDR.BIT.B2   //NO.41
#define RSA_TASK_SWICH3         PORT3.PIDR.BIT.B1   //NO.43


#define OPENMV_WORK_ENABLE_PIN  PORT4.PODR.BIT.B7   //NO.49
#define OPENMV_TASK_SWICH1      PORTB.PODR.BIT.B1   //NO.25
#define OPENMV_TASK_SWICH2      PORTB.PODR.BIT.B3   //NO.23
#define OPENMV_TASK_SWICH3      PORTB.PODR.BIT.B4   //NO.21

#define SYSTEM_BOOTUP_ALARM     PORT7.PODR.BIT.B6   //NO.33
#define BLINK_CONTROL			PORT7.PODR.BIT.B4	//NO.35
#define WORK_INDICATOR_LIGHT    PORT7.PODR.BIT.B2	//NO.37

#define SYSTEM_BOOTUP          RSA_WORK_ENABLE_PIN   //NO.55

#define TEMP_STOP_PORT          PORT7.PIDR.BIT.B1	//NO.38
#define TEMP_DIRECTION_PORT1	PORT7.PIDR.BIT.B3	//NO.36
#define TEMP_DIRECTION_PORT2 	PORT7.PIDR.BIT.B5	//NO.34

/************direction define**********/
#define FORWARD		0
#define BACKWARD	1
#define LEFT_BACK   2
#define RIGHT_BACK  3
#define LEFT		4
#define RIGHT 		5

/*****const define********/
#define Pi 3.1416
#define TASK_HEIGHT		   		 1.0f
#define GENERAL_HEIGHT			 0.7f
#define ENABLE_HEIGHT			 0.4f
#define LAND_HEIGHT              0.6f
#define PID_HEIGHT				 0.7f
#define TASK_ERROR_HEIGHT   	 1.2f

#define TASK1_X_SPEED		     0.0f
#define TASK3_X_SPEED			 0.12f
#define TASK4_X_SPEED            -0.2f

#define TASK5_FORWARD_X_SPEED	 0.2f
#define TASK5_BACKWARD_X_SPEED	 -0.2f
#define TASK5_LEFT_Y_SPEED		 -0.2f
#define TASK5_RIGHT_Y_SPEED		 0.2f

#define TASK_DELAY				 5000
#define LAND_DELAY				 2000

#define x_ITerm_MAX				 80.0f
#define y_ITerm_MAX				 80.0f

#define SET_POINT 				1
#define FINDING_CAR				2
#define FOLLOWING_CAR			3

/**********pid sampleTime********/
#define TASK1_SAMPLE_TIME     40
//#define TASK2_SAMPLE_TIME     30
#define TASK3_SAMPLE_TIME     30
#define TASK4_SAMPLE_TIME     40
#define TASK5_SAMPLE_TIME     40

/*********task flags************/
#define TASK1  1
#define TASK2  2
#define TASK3  3
#define TASK4  4
#define TASK5  5
#define TASK6  6
#define TASK7  7
// #define TASK8  8


/*********** data refering*************/
//define openmv_data means
#define ERROR_FLAG 0
#define LAND_FLAG  1

#define SITE_X     2
#define SITE_Y 	   3
#define CAR_X      4
#define CAR_Y      5

#define DATA_READY 6


/********error threshold*******/
#define TASK_ERROR_THRESHOLD 40

/*********EVENT & DATA & ERROR*****/
#define MAV_STATUS_INIT 0
#define MAV_STATUS_TAKEOFF 1
#define MAV_STATUS_FLYING 2
#define MAV_STATUS_PRELAND 3
#define MAV_STATUS_OVERFLY 4
#define MAV_STATUS_ERROR 255

//define system envet
#define EVENT_BOOTUP 			1
#define EVENT_STARTBUTTON 		2
#define EVENT_NOLOCALPOSITION 	4
#define EVENT_ARMCHECK 			8
#define EVENT_TAKEOFF 			16
#define EVENT_OPENMVBOOTUP 		32
#define EVENT_WAIT_NEW_DATA     64
#define EVENT_SEND_HEARTBEAT    128
#define EVENT_LANDED            256

#define EVENT_XUNXIAN 1
#define EVENT_PRELAND 1
#define EVENT_OVERFLY 1
#define EVENT_LAND 1


//define system error
#define ERROR_TASK_NUMBER			 65
#define ERROR_SPI_DATA 	  			 66
#define ERROR_UNKNOWN_PLACE 		 67
#define ERROR_CANNOT_GET_DATA 		 68
#define ERROR_WRONG_ORDER  			 69
#define ERROR_TASK_TIMEOUT			 70
#define ERROR_OPENMV_CRASH      	 71
#define ERROR_OPENMV_DATA_ABNORMAL   72

//define systerm data
#define DATA_OPENMV  			 97
#define DATA_X_OFFSET_RESULT 	 98
#define DATA_Y_OFFSET_RESULT 	 99
#define DATA_PID_X_SPEED 		 100
#define DATA_PID_Y_SPEED		 101
#define DATA_ATTITUDE 			 102
#define DATA_PID_PARAMETERS	 	 103
#define DATA_TASK_NUMBER         104
/* End user code. Do not edit comment generated here */
#endif
