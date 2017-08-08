/*
 * offset_calculate.c
 *
 *  Created on: 2017年7月26日
 *      Author: lenovo
 */


/******************************/
/*********imortant*************/
#include "r_cg_macrodriver.h"
/******************************/
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "offset_calculate.h"
#include "r_cg_userdefine.h"


/*********Renesas get PIX attiaude*********/
//float ** ras_get_pix_attitude = NULL; //&temp_pointer;
/******************************************/
/***********for tesst************/
float data[3] = {0.0, 0.0, 0.0};
float * temp_pointer = data;
float ** ras_get_pix_attitude = &temp_pointer;


float rasAngleCalculate(float offset)
{
    float angle;
    angle = atan(offset/162.71)*180/Pi;
    return angle;
}
float rasY_offsetCalculate(float openmv_x_offset)
{
//    float angle, y_offset = 0.0,  *height= NULL;
	float angle, y_offset = 0.0,h= 0.7, *height=&h;
    angle = rasAngleCalculate(openmv_x_offset - 80.0);
//    ras_get_pix_attitude = getAttitude();
//    height = get_height();
    y_offset = -((*height) * tan( (*ras_get_pix_attitude)[0]))/cos( (*ras_get_pix_attitude)[1] );
    y_offset += ((*height) * sin(angle*Pi/180) * sin((angle + 90 - (*ras_get_pix_attitude)[0]*180/Pi)*Pi/180))/(cos((*ras_get_pix_attitude)[0]) * cos((*ras_get_pix_attitude)[1]));
    return y_offset*100.0;
}
float rasX_offsetCalculate(float openmv_y_offset)
{
//    float angle, x_offset, *height = NULL;
	float angle, x_offset = 0.0,h= 0.7, *height=&h;
    angle = rasAngleCalculate(openmv_y_offset - 60.0);
//    ras_get_pix_attitude = getAttitude();
//    height = get_height();
    x_offset = (*height) * tan( (*ras_get_pix_attitude)[1]);
    x_offset += -((*height) * sin(angle*Pi/180))/(cos( (*ras_get_pix_attitude)[1]) * sin((90 - (*ras_get_pix_attitude)[1]*180/Pi + angle)*Pi/180));
    return x_offset*100.0;
}
