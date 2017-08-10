/*
 * blink.c
 *
 *  Created on: 2017年8月9日
 *      Author: lenovo
 */
#include "blink.h"
#include "r_cg_cmt.h"
#include "r_cg_userdefine.h"
#include "r_cg_macrodriver.h"

void distance_blink(void)
{
	BLINK_CONTROL = 0x01;
	delay_ms(150);
	BLINK_CONTROL = 0x00;
	delay_ms(150);
}

void work_blink(void)
{
	int count = 0;
	for(count = 0; count <7; count++)
	{
		WORK_INDICATOR_LIGHT = 0x01;
		delay_ms(120);
		WORK_INDICATOR_LIGHT = 0x00;
		delay_ms(120);
	}
}
