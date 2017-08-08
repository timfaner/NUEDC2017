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
* File Name    : r_cg_cmt.c
* Version      : Code Generator for RX23T V1.00.01.01 [30 Oct 2015]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for CMT module.
* Creation Date: 2017/8/6
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
#include "r_cg_cmt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile unsigned long millis_us = 0;
volatile unsigned long millis_ms = 0;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CMT2_Create
* Description  : This function initializes the CMT2 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT2_Create(void)
{
    /* Disable CMI2 interrupt */
    IEN(CMT2,CMI2) = 0U;
    
    /* Cancel CMT stop state in LPC */
    MSTP(CMT2) = 0U;

    /* Set control registers */
    CMT2.CMCR.WORD = _0000_CMT_CMCR_CKS_PCLK8 | _0040_CMT_CMCR_CMIE_ENABLE | _0080_CMT_CMCR_DEFAULT;
    CMT2.CMCOR = _0004_CMT2_CMCOR_VALUE;

    /* Set CMI2 priority level */
    IPR(CMT2,CMI2) = _0F_CMT_PRIORITY_LEVEL15;
}
/***********************************************************************************************************************
* Function Name: R_CMT2_Start
* Description  : This function starts the CMT2 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT2_Start(void)
{
    /* Enable CMI2 interrupt in ICU */
    IEN(CMT2,CMI2) = 1U;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 1U;
}
/***********************************************************************************************************************
* Function Name: R_CMT2_Stop
* Description  : This function stops the CMT2 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT2_Stop(void)
{
    /* Disable CMI2 interrupt in ICU */
    IEN(CMT2,CMI2) = 0U;

    /* Stop CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0U;
}
/***********************************************************************************************************************
* Function Name: R_CMT3_Create
* Description  : This function initializes the CMT3 channel.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT3_Create(void)
{
    /* Disable CMI3 interrupt */
    IEN(CMT3,CMI3) = 0U;
    
    /* Cancel CMT stop state in LPC */
    MSTP(CMT3) = 0U;

    /* Set control registers */
    CMT3.CMCR.WORD = _0000_CMT_CMCR_CKS_PCLK8 | _0040_CMT_CMCR_CMIE_ENABLE | _0080_CMT_CMCR_DEFAULT;
    CMT3.CMCOR = _1387_CMT3_CMCOR_VALUE;

    /* Set CMI3 priority level */
    IPR(CMT3,CMI3) = _0F_CMT_PRIORITY_LEVEL15;
}
/***********************************************************************************************************************
* Function Name: R_CMT3_Start
* Description  : This function starts the CMT3 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT3_Start(void)
{
    /* Enable CMI3 interrupt in ICU */
    IEN(CMT3,CMI3) = 1U;

    /* Start CMT3 count */
    CMT.CMSTR1.BIT.STR3 = 1U;
}
/***********************************************************************************************************************
* Function Name: R_CMT3_Stop
* Description  : This function stops the CMT3 channel counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CMT3_Stop(void)
{
    /* Disable CMI3 interrupt in ICU */
    IEN(CMT3,CMI3) = 0U;

    /* Stop CMT3 count */
    CMT.CMSTR1.BIT.STR3 = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
unsigned long millis(void)
{
	unsigned long _millis;
	_millis = millis_ms;
	return _millis;
}
unsigned long millis_us_function(void)
{
	unsigned long _millis_us;
	_millis_us = millis_us;
	return _millis_us;
}
void delay_us(unsigned int us)
{
	unsigned long present_time;
	present_time = millis_us_function();
	while((millis_us_function() - present_time) <= us);
}
void delay_ms(unsigned int ms)
{
	unsigned long present_time;
	present_time = millis();
	while((millis() - present_time) <= ms);
}
/* End user code. Do not edit comment generated here */
