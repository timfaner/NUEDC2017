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
#include "17_task4.h"
#include "17_task5.h"
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
double task1_x_kp=5.50, task1_x_ki=0.70, task1_x_kd=0.3,
	   task1_y_kp = 5.50, task1_y_ki = 0.80, task1_y_kd = 0.3;

double task3_x_kp=5.50, task3_x_ki=0.70, task3_x_kd=0.30,
	   task3_y_kp = 5.50, task3_y_ki = 0.80, task3_y_kd = 0.30;

double task4_x_kp=5.50, task4_x_ki=0.70, task4_x_kd=0.30,
	   task4_y_kp = 5.50, task4_y_ki = 0.80, task4_y_kd = 0.30;

double task5_x_kp=4.50, task5_x_ki=0.70, task5_x_kd=0.20,
	   task5_y_kp = 5.50, task5_y_ki = 0.80, task5_y_kd = 0.30;

/***************functions******************/
void task6(void);
void task7(void);
float read_height(void);
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

/************commder from car***********/
volatile uint8_t car_cmd = 0;


/***********for key************/
uint8_t bootup_key = 0;

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

	//initial
	R_MAIN_UserInit();
   /* Start user code. Do not edit comment generated here */
	//Wait  for One_key StartUp
	while(!bootup_key)
	{
		uart_5_printf("Task Number: %d Wait system start working\n",task_number);
		if(!SYSTEM_BOOTUP)
		{
			delay_ms(5);
			if(SYSTEM_BOOTUP == 0)
				bootup_key = 1;
			else
				bootup_key = 0;
		}
		delay_ms(500);
	}
	WORK_INDICATOR_LIGHT = 0x01;

    task_number = rasTaskSwitch();
    rasCmdToOpenmv(task_number); //切换openmv任务

	alarm_bibi();


    switch (task_number){
    	case TASK1:
    		xSetTunings(task1_x_kp, task1_x_ki, task1_x_kd);
    		ySetTunings(task1_y_kp, task1_y_ki, task1_y_kd);
    		xSetSampleTime(TASK1_SAMPLE_TIME);
    		ySetSampleTime(TASK1_SAMPLE_TIME);
    		task1();
    		break;
    	case TASK2:
    		task2();
    		break;
    	case TASK3:
    		xSetTunings(task3_x_kp, task3_x_ki, task3_x_kd);
    		ySetTunings(task3_y_kp, task3_y_ki, task3_y_kd);
    		xSetSampleTime(TASK3_SAMPLE_TIME);
    		ySetSampleTime(TASK3_SAMPLE_TIME);
    		task3();
    		break;
    	case TASK4:
    		xSetTunings(task4_x_kp, task4_x_ki, task4_x_kd);
    		ySetTunings(task4_y_kp, task4_y_ki, task4_y_kd);
    		xSetSampleTime(TASK4_SAMPLE_TIME);
    		ySetSampleTime(TASK4_SAMPLE_TIME);
    		task4();
    		break;
    	case TASK5:
    		xSetTunings(task5_x_kp, task5_x_ki, task5_x_kd);
    		ySetTunings(task5_y_kp, task5_y_ki, task5_y_kd);
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

	debug_text("start!\n");

//	buzzer_alarm();
	work_blink();


	init(SCI1_Serial_Send,delay_ms,millis,uart_5_printf);
	flag_data_updated=getFlagDataUpdated();
	apm_attitude=getAttitude();
	apm_height = get_height();
//	wait_link();
	requestDataStream(500,50,50);
	debug_text("\nRx Initialized\n");


	xPID(&x_input, &x_output, &setpoint, task1_x_kp, task1_x_ki, task1_x_kd, DIRECT);
	xSetMode(AUTOMATIC);
	yPID(&y_input, &y_output, &setpoint, task1_y_kp, task1_y_ki, task1_y_kd, DIRECT);
	ySetMode(AUTOMATIC);


//	rasWirelessAdjustParameters();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */

void task6(void)
{
	debug_text(" no commder of task6\n");
}
void task7(void)
{
	debug_text(" no commder of task7\n");
}
float read_height(void)
{
	float * temp = NULL;
	temp = get_height();
	return *temp;
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
