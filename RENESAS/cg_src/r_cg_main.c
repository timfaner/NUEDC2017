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
* File Name    : r_cg_main.c
* Version      : Code Generator for RX23T V1.00.01.01 [30 Oct 2015]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements main function.
* Creation Date: 2017/7/17
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
#include "r_cg_rspi.h"
/* Start user code for include. Do not edit comment generated here */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

#include "17_task1.h"
#include "17_task2.h"
#include "17_task3.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/*************spi parameters**********************/
uint8_t rx_data = 0;
uint8_t * rx_buffer = &rx_data;
volatile uint8_t openmv_data[7] = {255,255,255,255,255,255,255};
//openmv_data  rror mav_statu x y landflag
volatile uint8_t openmv_data_flow[9] = {0,1,2,3,4,5,6,7,8};
/*************************************************/

/**********system.h define***********/
volatile uint16_t system_event;
volatile uint8_t system_error_code;

/*********pid parameters********/
double x_input= 0.0, x_output, setpoint=0.0,
		y_input = 0.0, y_output;
double x_kp=7.50, x_ki=0.0, x_kd=0.0,
	   y_kp = 7.50, y_ki = 0.0, y_kd = 0.0;

/***************functions******************/
void task5(void);
void task6(void);
void task7(void);
void task8(void);
//void rasWirelessAdjustParameters(void);
/******************************************/

/************get data from apm*************/
//int armflag=0;
int * flag_data_updated = NULL;
float** apm_attitude = NULL;
float * apm_height = NULL;
unsigned long last_heartbeat_time=0;
unsigned long runtime=0;
/***************************************/


/**********timer for tasks*********/
unsigned long task_cycle_timer = 0;
unsigned long task_cycle_time_monitor = 0;

/**********flag for error handle*********/
uint8_t last_error_flag=0;
uint16_t flying_count=0;
volatile uint8_t openmv_error_flag = 0;
volatile uint8_t openmv_wrong_order = 0;

/* End user code. Do not edit comment generated here */

void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
 	uint8_t task_number = 0;
	uint8_t cmd = 0;
	uint8_t arm_flag=0;
	uint8_t takeoff_flag=0;

	//initial
	R_MAIN_UserInit();
   /* Start user code. Do not edit comment generated here */
    task_number = rasTaskSwitch();
    rasCmdToOpenmv(task_number); //切换openmv任务
	//倒计时
	//wait command cycle
	while(!(arm_flag && takeoff_flag)){
		debug_text("wait for commder\n");
		if(sci5_receive_available()){
			SCI5_Serial_Receive(&cmd, 1);
			if(cmd == 'a')
			{
				set_pid(0, 1800.0f, 400.0f, 150.0f, 600.0f);
				set_pid(1, 2000.0f, 300.0f, 200.0f, 400.0f);
				if(armcheck())
				{
					arm_flag=1;
				}
				systemEventUpdate(EVENT_ARMCHECK);
				debug_text("arm check!\n");
				cmd = '\0';
			}
			if(cmd == 't')
			{
				if(mav_takeoff(TASK_HEIGHT))
				{
					takeoff_flag=1;
				}
				systemEventUpdate(EVENT_TAKEOFF);
				debug_text("take off!\n");
				cmd = '\0';
			}
		}
		delay_ms(600);
	}

///**************important******************/
	while (*(apm_height) < TASK_HEIGHT-0.1)
		{
			delay_ms(100);
			uart_5_printf("height: %f  wait for Set Height\n",*apm_height);
		}
	//set point
	set_new_vel(0.0, 0.0, TASK_HEIGHT);

	rasCmdToOpenmv(task_number);

	OPENMV_WORK_ENABLE_PIN = 1; //通知openmv开始工作 将该引脚置高
	delay_ms(600);  //wait openmv initialize
	set_new_vel(0.0, 0.0, TASK_HEIGHT);

	debug_text("openmv initialized");
    switch (task_number){
    	case TASK1:
    		xSetSampleTime(TASK1_SAMPLE_TIME);
    		ySetSampleTime(TASK1_SAMPLE_TIME);
    		task1();
    		break;
    	case TASK2:
    		task2();
    		break;
    	case TASK3:
    		xSetSampleTime(TASK3_SAMPLE_TIME);
    		ySetSampleTime(TASK3_SAMPLE_TIME);
    		task3();
    		break;
    	case TASK4:
    		xSetSampleTime(TASK4_SAMPLE_TIME);
    		ySetSampleTime(TASK4_SAMPLE_TIME);
    		task4();
    		break;
    	case TASK5:
    		xSetSampleTime(TASK5_SAMPLE_TIME);
    		ySetSampleTime(TASK5_SAMPLE_TIME);
			task5();
    		break;
    	case TASK6:
    		task6();
    		break;
    	case TASK7:
    		task7();
    		break;
    	case TASK8:
    		task8();
    		break;
    	case -1:
    		debug_text("wrong task number (from main)\n");
    		break;
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
	SCI1_Start();
	SCI5_Start();
	R_CMT2_Start();
	R_CMT3_Start();
	R_RSPI0_Start();
	spiReceive(rx_buffer);

	while(!SYSTEM_BOOTUP)
	{
		debug_text("wait system bootup\n");
		delay_ms(50);
	}
	alarm_bibi();
	work_blink();
	WORK_INDICATOR_LIGHT = 0x01;

	init(SCI1_Serial_Send,delay_ms,millis,uart_5_printf);
	flag_data_updated=getFlagDataUpdated();
	apm_attitude=getAttitude();
	apm_height = get_height();
	wait_link();
	requestDataStream(500,50,50);
	debug_text("\nRx Initialized\n");

	xPID(&x_input, &x_output, &setpoint, x_kp, x_ki, x_kd, DIRECT);
	xSetMode(AUTOMATIC);
	yPID(&y_input, &y_output, &setpoint, y_kp, y_ki, y_kd, DIRECT);
	ySetMode(AUTOMATIC);

//	rasWirelessAdjustParameters();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void task5(void)
{
	debug_text(" no commder of task5\n");
}
void task6(void)
{
	debug_text(" no commder of task6\n");
}
void task7(void)
{
	debug_text(" no commder of task7\n");
}
void task8(void)
{
	debug_text(" no commder of task8\n");
}
/*
void rasWirelessAdjustParameters(void)
{
	unsigned char ch, *sci_Kp = &kp, *sci_Ki = &ki, *sci_Kd = &kd, temp[10];
	uint8_t i = 0, flag = 0;
	debug_text("Parameters adjust\n");
	while(!sci5_receive_available());
	SCI5_Serial_Receive(&ch, 1);
	if(ch == 'Y')
	{
		debug_text("input Kp:\n");
		while(!sci5_receive_available());
		while(sci5_receive_available())
		{
			debug_text("rev ok\n");
			SCI5_Serial_Receive(sci_Kp, 5);
//			uart_5_printf("\n received ch %c \n", kp);
//			if(*(sci_Kp+4) == '#') flag=1;
//			while(!sci5_receive_available());
		}
		kp = atof(*sci_Kp);
		uart_5_printf("\n received kp %f \n", kp);

		debug_text("input Ki:\n");
		while(!sci5_receive_available());
		i = 0;
		while(SCI5_Serial_Receive(&ch, 1) != '#')
		{
			*(sci_Ki + i) = ch;
			i++;
		}
		ki = atof(*sci_Ki);
		uart_5_printf("\n received ki %f \n", ki);

		debug_text("input Kd:\n");
		while(!sci5_receive_available());
		i = 0;
		while(SCI5_Serial_Receive(&ch, 1) != '#')
		{
			*(sci_Kp + i) = ch;
			i++;
		}
		kd = atof(*sci_Kd);
		uart_5_printf("\n received kd %c \n", ch);

		debug_text("Parameters changed!\n");
	}
	else if(ch == 'N')
	{
		uart_5_printf("use the default parameters: kp %f ki %f kd %f\n",kp,ki,kd);
	}
	else
	{
		debug_text("cmd error\n");
	}
}  */
/* End user code. Do not edit comment generated here */
