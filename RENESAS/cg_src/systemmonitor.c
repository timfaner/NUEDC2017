
#include "r_cg_cmt.h"
#include "r_cg_rspi.h"
#include "r_cg_sci.h"
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "systemmonitor.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


#define SPACE 32
#define ENDL 10


uint8_t* package=NULL;

//发送指定数据 分别是数据数组 数据个数 类型
void systemMonitor(uint8_t *arg,int arg_count,uint8_t data_type){
    static uint8_t send_counter = 'a';
    uint8_t send_counter_str[4];
    uint8_t* arg_str = NULL;
    unsigned char space[2] = {" "};
    unsigned char endl[2] = {"\n"};

    int length=0;
    unsigned char * text=NULL;
    
    unsigned char data_type_str[4];
    
    volatile unsigned long current_time = 0;
    unsigned char current_time_str[12]={0};
    unsigned char begin_srt[2] = {13,0};
    int i;
    //避免序列中出现 \x00,\n
    if((send_counter == 0) || (send_counter == 10))
        send_counter++;

    package =  (uint8_t*)malloc(arg_count + 25);

    for(i=0;i<arg_count+25;i++){
    	*(package+i)='\0';
    }
    arg_str = (uint8_t*)malloc(arg_count+1);
    for(i = 0;i<arg_count;i++){
         *(arg_str+i) = *(arg+i) ;
    }
    //*(arg_str+arg_count) = NULL;
    *(arg_str+arg_count) = '\0';
    
    
    current_time = millis()+3;
    sprintf(current_time_str,"%lu",current_time);
    

    
    send_counter_str[0] = send_counter;
    send_counter_str[1] = NULL;
    
    
    data_type_str[0] = data_type;
    data_type_str[1] = NULL;
    
    strcat(package,begin_srt);
    strcat(package,current_time_str);
    strcat(package,space);
    strcat(package,send_counter_str);
    strcat(package,data_type_str);
    strcat(package,arg_str);
    strcat(package,endl);
	text = package;
	while(*text!='\0'){
		length++;
		text++;
	}


	SCI5_Serial_Send(package,length);
    
    free(package);
    package = NULL;
    free(arg_str);
    arg_str = NULL;
    
    send_counter++;
}


void systemDataUpdate(float *data,int data_count,uint8_t data_type){
    unsigned char package[12][20];
    unsigned char send[50]={0} ;
    int i = 0;

    int length =0; //记得初始化
    unsigned char * text = send;
    char comma[2] = ",";
    
    for(i = 0;i< data_count-1;i++){
        sprintf(package[i],"%.2f",*(data+i));
        strcat(package[i],comma);
        strcat(send,package[i]);
    }
    sprintf(package[data_count-1],"%.2f",*(data+data_count-1));
    strcat(send,package[data_count-1]);

    
    while(*text!='\0'){
        length++;
        text++;
    }
    systemMonitor(send,length,data_type);
}

void systemEventUpdate(uint16_t new_event){
    uint8_t package[2];
    
    if ((system_event & new_event) == 0)
      system_event |= new_event;
    else 
      system_event ^= new_event;
    
    package[0] =(uint8_t) (system_event & 0xff);
    package[1] =(uint8_t) ((system_event>>8) & 0xff);
    if(package[1]== 0)
        systemMonitor(package,1,MONITOR_EVENT);
    else
        systemMonitor(package,2,MONITOR_EVENT);
}

void systemErrorUpdate(uint8_t error){
    uint8_t errorcode;
    errorcode = error;
    systemMonitor(&errorcode,1,MONITOR_ERROR);
}
