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
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sci_user.c
* Version      : Code Generator for RX23T V1.00.04.02 [29 Nov 2016]
* Device(s)    : R5F523T5AxFM
* Tool-Chain   : CCRX
* Description  : This file implements device driver for SCI module.
* Creation Date: 2017/7/8
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
#include "r_cg_sci.h"
/* Start user code for include. Do not edit comment generated here */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

/* Start user code for global. Do not edit comment generated here */
struct Circle_Queue sci1_rx_circle[RX_BUFFER_MAX_VOLUME];
struct Circle_Queue * current_sci1_rx_addr;
struct Circle_Queue * read_sci1_rx_addr;

volatile uint16_t sci1_rx_count = 0;

struct Tx_Circle_Queue sci1_tx_circle[TX_QUEUE_MAX_SIZE];
struct Tx_Circle_Queue * current_sci1_tx_addr;
struct Tx_Circle_Queue * wait_sci1_tx_addr;

volatile uint16_t sci1_tx_count = 0;

extern volatile uint8_t sci1_tx_idle_token;

struct Circle_Queue sci5_rx_circle[RX_BUFFER_MAX_VOLUME];
struct Circle_Queue * current_sci5_rx_addr;
struct Circle_Queue * read_sci5_rx_addr;
volatile uint16_t sci5_rx_count = 0;

struct Tx_Circle_Queue sci5_tx_circle[TX_QUEUE_MAX_SIZE];
struct Tx_Circle_Queue * current_sci5_tx_addr;
struct Tx_Circle_Queue * wait_sci5_tx_addr;

volatile uint16_t sci5_tx_count = 0;

extern volatile uint8_t sci5_tx_idle_token;

//
static uint8_t sci1_receive_temp[1] = {0};
static uint8_t sci1_receive_char(void);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_sci1_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TXI1
#pragma interrupt r_sci1_transmit_interrupt(vect=VECT(SCI1,TXI1),fint)
#else
#pragma interrupt r_sci1_transmit_interrupt(vect=VECT(SCI1,TXI1))
#endif
static void r_sci1_transmit_interrupt(void)
{
    if (current_sci1_tx_addr->count > 0U)
    {
        SCI1.TDR = current_sci1_tx_addr->array[current_sci1_tx_addr->sent];
        current_sci1_tx_addr->sent++;
        current_sci1_tx_addr->count--;
        if(current_sci1_tx_addr->count <= 0U)
        {
        	current_sci1_tx_addr->sent = 0;
			current_sci1_tx_addr->count = 0;
			free(current_sci1_tx_addr->array);

			sci1_tx_count--;

			current_sci1_tx_addr = current_sci1_tx_addr->next;
        }
    }
    else
    {
    	//SCI1.SCR.BIT.TIE = 0U;
        SCI1.SCR.BIT.TIE = 0U;
        SCI1.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_sci1_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TEI1
#pragma interrupt r_sci1_transmitend_interrupt(vect=VECT(SCI1,TEI1),fint)
#else
#pragma interrupt r_sci1_transmitend_interrupt(vect=VECT(SCI1,TEI1))
#endif
static void r_sci1_transmitend_interrupt(void)
{
    /* Set TXD1 pin */
    PORTD.PMR.BYTE &= 0xF7U;
    SCI1.SCR.BIT.TIE = 0U;
    SCI1.SCR.BIT.TE = 0U;
    SCI1.SCR.BIT.TEIE = 0U;

    r_sci1_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci1_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_RXI1
#pragma interrupt r_sci1_receive_interrupt(vect=VECT(SCI1,RXI1),fint)
#else
#pragma interrupt r_sci1_receive_interrupt(vect=VECT(SCI1,RXI1))
#endif
static void r_sci1_receive_interrupt(void)
{
	current_sci1_rx_addr->data = SCI1.RDR;
	current_sci1_rx_addr = current_sci1_rx_addr->next;
	sci1_rx_count++;

	if(sci1_rx_count > RX_BUFFER_MAX_VOLUME)  //throw the old data
	{
		read_sci1_rx_addr = read_sci1_rx_addr->next;
		sci1_rx_count--;
	}

	 receive(sci1_receive_char());
}
/***********************************************************************************************************************
* Function Name: r_sci1_receiveerror_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI1_ERI1
#pragma interrupt r_sci1_receiveerror_interrupt(vect=VECT(SCI1,ERI1),fint)
#else
#pragma interrupt r_sci1_receiveerror_interrupt(vect=VECT(SCI1,ERI1))
#endif
static void r_sci1_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_sci1_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI1.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI1.SSR.BYTE = err_type;
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_transmitend
* Description  : This function is a callback function when SCI1 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
	sci1_tx_idle_token = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_receiveend
* Description  : This function is a callback function when SCI1 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci1_callback_receiveerror
* Description  : This function is a callback function when SCI1 reception encounters error.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci1_callback_receiveerror(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci5_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TXI5
#pragma interrupt r_sci5_transmit_interrupt(vect=VECT(SCI5,TXI5),fint)
#else
#pragma interrupt r_sci5_transmit_interrupt(vect=VECT(SCI5,TXI5))
#endif
static void r_sci5_transmit_interrupt(void)
{
	if (current_sci5_tx_addr->count > 0U)
	{
		SCI5.TDR = current_sci5_tx_addr->array[current_sci5_tx_addr->sent];
		current_sci5_tx_addr->sent++;
		current_sci5_tx_addr->count--;
		if(current_sci5_tx_addr->count <= 0U)
		{
			current_sci5_tx_addr->sent = 0;
			current_sci5_tx_addr->count = 0;
			free(current_sci5_tx_addr->array);

			sci5_tx_count--;

			current_sci5_tx_addr = current_sci5_tx_addr->next;
		}
	}
	else
	{
		SCI5.SCR.BIT.TIE = 0U;
		SCI5.SCR.BIT.TEIE = 1U;
	}
}

/***********************************************************************************************************************
* Function Name: r_sci5_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_TEI5
#pragma interrupt r_sci5_transmitend_interrupt(vect=VECT(SCI5,TEI5),fint)
#else
#pragma interrupt r_sci5_transmitend_interrupt(vect=VECT(SCI5,TEI5))
#endif
static void r_sci5_transmitend_interrupt(void)
{
    /* Set TXD5 pin */
    PORTB.PMR.BYTE &= 0xDFU;
    SCI5.SCR.BIT.TIE = 0U;
    SCI5.SCR.BIT.TE = 0U;
    SCI5.SCR.BIT.TEIE = 0U;

    r_sci5_callback_transmitend();
}
/***********************************************************************************************************************
* Function Name: r_sci5_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_RXI5
#pragma interrupt r_sci5_receive_interrupt(vect=VECT(SCI5,RXI5),fint)
#else
#pragma interrupt r_sci5_receive_interrupt(vect=VECT(SCI5,RXI5))
#endif
static void r_sci5_receive_interrupt(void)
{
	//always get the data
	current_sci5_rx_addr->data = SCI5.RDR;
	//emergency stop
	/*if(current_sci5_rx_addr->data == 's')
	{
		R_SCI1_Stop();
		debug_text("SCI1 stopped\n");
	}
	if(current_sci5_rx_addr->data == 'i')
	{
		R_SCI1_Start();
		SCI1.SCR.BIT.RIE = 1U;
		SCI1.SCR.BIT.RE = 1U;
		debug_text("SCI1 reinitiated\n");
	}*/
	//
	current_sci5_rx_addr = current_sci5_rx_addr->next;
	sci5_rx_count++;

	if(sci5_rx_count > RX_BUFFER_MAX_VOLUME)  //throw the old data
	{
		read_sci5_rx_addr = read_sci5_rx_addr->next;
		sci5_rx_count--;
	}
}
/***********************************************************************************************************************
* Function Name: r_sci5_receiveerror_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#if FAST_INTERRUPT_VECTOR == VECT_SCI5_ERI5
#pragma interrupt r_sci5_receiveerror_interrupt(vect=VECT(SCI5,ERI5),fint)
#else
#pragma interrupt r_sci5_receiveerror_interrupt(vect=VECT(SCI5,ERI5))
#endif
static void r_sci5_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_sci5_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI5.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI5.SSR.BYTE = err_type;
}
/***********************************************************************************************************************
* Function Name: r_sci5_callback_transmitend
* Description  : This function is a callback function when SCI5 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci5_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
	sci5_tx_idle_token = 1;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci5_callback_receiveend
* Description  : This function is a callback function when SCI5 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci5_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_sci5_callback_receiveerror
* Description  : This function is a callback function when SCI5 reception encounters error.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_sci5_callback_receiveerror(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
extern uint16_t sci1_receive_available(void)
{
	return sci1_rx_count;
}

void sci1_rx_circle_init(void)   //create the circle
{
	int i;
	for(i = 0;i < RX_BUFFER_MAX_VOLUME-1;i++)
		sci1_rx_circle[i].next = &sci1_rx_circle[i+1];
	sci1_rx_circle[RX_BUFFER_MAX_VOLUME-1].next = &sci1_rx_circle[0];

	for(i = 0;i < RX_BUFFER_MAX_VOLUME;i++)
		sci1_rx_circle[i].data = 0;

	current_sci1_rx_addr = &sci1_rx_circle[0];
	read_sci1_rx_addr = current_sci1_rx_addr;
}

void sci1_tx_circle_init(void)   //create the circle
{
	int i;
	for(i = 0;i < TX_QUEUE_MAX_SIZE-1;i++)
		sci1_tx_circle[i].next = &sci1_tx_circle[i+1];
	sci1_tx_circle[TX_QUEUE_MAX_SIZE-1].next = &sci1_tx_circle[0];

	for(i = 0;i < TX_QUEUE_MAX_SIZE;i++)
	{
		sci1_tx_circle[i].count = 0;
		sci1_tx_circle[i].sent = 0;
	}
	current_sci1_tx_addr = &sci1_tx_circle[0];
	wait_sci1_tx_addr = current_sci1_tx_addr;
}

extern uint16_t sci5_receive_available(void)
{
	return sci5_rx_count;
}

void sci5_rx_circle_init(void)   //create the circle
{
	int i;
	for(i = 0;i < RX_BUFFER_MAX_VOLUME-1;i++)
		sci5_rx_circle[i].next = &sci5_rx_circle[i+1];
	sci5_rx_circle[RX_BUFFER_MAX_VOLUME-1].next = &sci5_rx_circle[0];

	for(i = 0;i < RX_BUFFER_MAX_VOLUME;i++)
		sci5_rx_circle[i].data = 0;

	current_sci5_rx_addr = &sci5_rx_circle[0];
	read_sci5_rx_addr = current_sci5_rx_addr;
}

void sci5_tx_circle_init(void)   //create the circle
{
	int i;
	for(i = 0;i < TX_QUEUE_MAX_SIZE-1;i++)
		sci5_tx_circle[i].next = &sci5_tx_circle[i+1];
	sci5_tx_circle[TX_QUEUE_MAX_SIZE-1].next = &sci5_tx_circle[0];

	for(i = 0;i < TX_QUEUE_MAX_SIZE;i++)
	{
		sci5_tx_circle[i].count = 0;
		sci5_tx_circle[i].sent = 0;
	}
	current_sci5_tx_addr = &sci5_tx_circle[0];
	wait_sci5_tx_addr = current_sci5_tx_addr;
}

static uint8_t sci1_receive_char(void)
{
	SCI1_Serial_Receive(sci1_receive_temp,1);
	return *sci1_receive_temp;
}
/* End user code. Do not edit comment generated here */
