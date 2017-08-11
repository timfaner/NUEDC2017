/*
 * buzzer.c
 *
 *  Created on: 2017年8月6日
 *      Author: lenovo
 */
#include "buzzer.h"
#include "r_cg_cmt.h"
#include "r_cg_userdefine.h"
#include "r_cg_macrodriver.h"

void pix_init_alarm(void)
{
	SYSTEM_BOOTUP_ALARM = 0;
	SYSTEM_BOOTUP_ALARM = 1;
	delay_ms(50);
	SYSTEM_BOOTUP_ALARM = 0;
	delay_ms(50);
	SYSTEM_BOOTUP_ALARM = 1;
	delay_ms(50);
	SYSTEM_BOOTUP_ALARM = 0;
	delay_ms(50);
	SYSTEM_BOOTUP_ALARM = 1;
	delay_ms(50);
	SYSTEM_BOOTUP_ALARM = 0;
	delay_ms(50);
}


void openmv_init_alarm(void)
{
	SYSTEM_BOOTUP_ALARM = 0;
	SYSTEM_BOOTUP_ALARM = 1;
	delay_ms(500);
	SYSTEM_BOOTUP_ALARM = 0;
}

void alarm_bibi(void)
{
	unsigned long count = 0;
   	for(count = 0; count <= 35; count++)
    	{
    		if(count<=7)
    		{
				SYSTEM_BOOTUP_ALARM  = 1;
				delay_ms(400);
				SYSTEM_BOOTUP_ALARM  = 0;
				delay_ms(400);
    		}
    		else if(count <= 31)
    		{
				SYSTEM_BOOTUP_ALARM  = 1;
				delay_ms(40);
				SYSTEM_BOOTUP_ALARM  = 0;
				delay_ms(40);
    		}
    		else
    		{
				SYSTEM_BOOTUP_ALARM  = 1;
				delay_ms(100);
				SYSTEM_BOOTUP_ALARM  = 0;
				delay_ms(100);
    		}
    	}
}
void distance_alarm(void)
{
    SYSTEM_BOOTUP_ALARM  = 1;
//   	SYSTEM_BOOTUP_ALARM  = 0;
}

void preland_alarm(void)
{
	SYSTEM_BOOTUP_ALARM = 1;
	delay_ms(100);
	SYSTEM_BOOTUP_ALARM = 0;
}

void land_alarm(void)
{
	SYSTEM_BOOTUP_ALARM = 1;
	delay_ms(60);
	SYSTEM_BOOTUP_ALARM = 0;
	delay_ms(20);
	SYSTEM_BOOTUP_ALARM = 1;
	delay_ms(60);
	SYSTEM_BOOTUP_ALARM = 0;
}
