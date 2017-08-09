/*
 * user_main.c
 *
 *  Created on: 2017年7月19日
 *      Author: lenovo
 */
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
#include "string.h"
#include "r_cg_userdefine.h"
int wait_link(){
	int link_flag=0;
	int i=0;
	int last_ping_time=millis();
	unsigned char link_list[5]={'\0'};
	while(!link_flag){
		 if(sci5_receive_available()){
			 for(i=0;i<3;i++){
				 link_list[i]=link_list[i+1];
			 }
		     SCI5_Serial_Receive(link_list+3,1);
		 }
		 if(!strcmp(link_list,"link"))
			 return 1;
		 if(millis()-last_ping_time>2000){
			 last_ping_time=millis();
			 debug_text("wait for link\n");
		 }
	}

}

