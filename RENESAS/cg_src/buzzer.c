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


void alarm(void)
{
	unsigned long cycle = 0;
	unsigned long count = 0;
	for(count = 0; count <= 19; count++)
	{
		if(count<=7)
		{
	    	for(cycle = 200; cycle <= 1000; cycle++)
	    	{
	    		SYSTEM_BOOTUP_ALARM = 1;
	    		delay_us(cycle);
	    		SYSTEM_BOOTUP_ALARM = 0;
	    		delay_us(cycle);
	    	}
	    	delay_ms(10);
		}
		else
		{
	    	for(cycle = 500; cycle >= 20; cycle--)
	    	{
	    		SYSTEM_BOOTUP_ALARM = 1;
	    		delay_us(cycle);
	    		SYSTEM_BOOTUP_ALARM = 0;
	    		delay_us(cycle);
	    	}
	    	delay_ms(4);
		}
	}
}
void alarm_bibi(void)
{
	int period = 0;
	unsigned long cycle = 0;
	unsigned long count = 0;
   	for(count = 0; count <= 35; count++)
    	{
    		period = 222;
    		if(count<=7)
    		{
				for(cycle = 0; cycle <= period; cycle++)
				{
					SYSTEM_BOOTUP_ALARM = 1;
					delay_us(cycle);
					SYSTEM_BOOTUP_ALARM = 0;
					delay_us(period-cycle);
				}
				 delay_ms(800);
    		}
    		else if(count <= 31)
    		{
				for(cycle = 0; cycle <= period; cycle++)
				{
					SYSTEM_BOOTUP_ALARM = 1;
					delay_us(cycle);
					SYSTEM_BOOTUP_ALARM = 0;
					delay_us(period-cycle);
				}
				 delay_ms(80);
    		}
    		else
    		{
				for(cycle = 0; cycle <= period; cycle++)
				{
					SYSTEM_BOOTUP_ALARM = 1;
					delay_us(cycle);
					SYSTEM_BOOTUP_ALARM = 0;
					delay_us(period-cycle);
				}
				 delay_ms(200);
    		}
    	}
}
