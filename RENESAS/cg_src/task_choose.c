/*
 * task_choose.c
 *
 *  Created on: 2017年8月8日
 *      Author: lenovo
 */

/*********imortant*************/
#include "r_cg_macrodriver.h"
/******************************/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "r_cg_sci.h"
#include "task_choose.h"
#include "r_cg_userdefine.h"

int rasTaskSwitch(void)
{

	if(!(RSA_TASK_SWICH1) && !(RSA_TASK_SWICH2) && !(RSA_TASK_SWICH3))
	{
		debug_text("\nchoose tast1\n");
		return TASK1;
	}
	else if(!(RSA_TASK_SWICH1) && !(RSA_TASK_SWICH2) && RSA_TASK_SWICH3)
	{
		debug_text("\nchoose tast2\n");
		return TASK2;
	}
	else if( !(RSA_TASK_SWICH1) && RSA_TASK_SWICH2 && !(RSA_TASK_SWICH3))
	{
		debug_text("\nchoose tast3\n");
		return TASK3;
	}
	else if( !(RSA_TASK_SWICH1) && RSA_TASK_SWICH2 && RSA_TASK_SWICH3)
	{
		debug_text("\nchoose tast4\n");
		return TASK4;
	}
	else if( RSA_TASK_SWICH1 && !(RSA_TASK_SWICH2) && !(RSA_TASK_SWICH3))
	{
		debug_text("\nchoose tast5\n");
		return TASK5;
	}
	else if( RSA_TASK_SWICH1 && !(RSA_TASK_SWICH2) && RSA_TASK_SWICH3)
	{
		debug_text("\nchoose tast6\n");
		return TASK6;
	}
	else if( RSA_TASK_SWICH1 && RSA_TASK_SWICH2 && !(RSA_TASK_SWICH3))
	{
		debug_text("\nchoose tast7\n");
		return TASK7;
	}
	else if( RSA_TASK_SWICH1 && RSA_TASK_SWICH2 && RSA_TASK_SWICH3)
	{
		debug_text("\nchoose tast8\n");
		return TASK8;
	}
	else
		return -1;
}

void rasCmdToOpenmv(uint8_t flag)
{
	if(flag ==TASK1)
	{
		OPENMV_TASK_SWICH1 = 0;
		OPENMV_TASK_SWICH2 = 0;
		OPENMV_TASK_SWICH3 = 0;
	}
	else if(flag ==TASK2)
	{
		OPENMV_TASK_SWICH1 = 0;
		OPENMV_TASK_SWICH2 = 0;
		OPENMV_TASK_SWICH3 = 1;
	}
	else if(flag ==TASK3)
	{
		OPENMV_TASK_SWICH1 = 0;
		OPENMV_TASK_SWICH2 = 1;
		OPENMV_TASK_SWICH3 = 0;
	}
	else if(flag ==TASK4)
	{
		OPENMV_TASK_SWICH1 = 0;
		OPENMV_TASK_SWICH2 = 1;
		OPENMV_TASK_SWICH3 = 1;
	}
	else if(flag ==TASK5)
	{
		OPENMV_TASK_SWICH1 = 1;
		OPENMV_TASK_SWICH2 = 0;
		OPENMV_TASK_SWICH3 = 0;
	}
	else if(flag ==TASK6)
	{
		OPENMV_TASK_SWICH1 = 1;
		OPENMV_TASK_SWICH2 = 0;
		OPENMV_TASK_SWICH3 = 1;
	}
	else if(flag ==TASK7)
	{
		OPENMV_TASK_SWICH1 = 1;
		OPENMV_TASK_SWICH2 = 1;
		OPENMV_TASK_SWICH3 = 0;
	}
	else if(flag ==TASK8)
	{
		OPENMV_TASK_SWICH1 = 1;
		OPENMV_TASK_SWICH2 = 1;
		OPENMV_TASK_SWICH3 = 1;
	}
	else
	{
//		systemErrorUpdate(ERROR_TASK_NUMBER);
		debug_text("error task number\n");
	}
}
