/*================================
 * ===========接线头文件============
 =================================*/


/****************蓝牙串口号**********/
#define _seriaL Serial
#define _seriaL_compass Serial3 //罗盘对应
/***************MPU6050************/
#define IMU_INT_PIN 1 //IMU中断


/**************电机****************/
#define motor_lf_in1 30
#define motor_lf_in2 32
#define motor_lf_en 4
#define motor_rf_in1 38
#define motor_rf_in2 40
#define motor_rf_en 5

#define motor_lb_in1 36
#define motor_lb_in2 34
#define motor_lb_en 6
#define motor_rb_in1 44
#define motor_rb_in2 42
#define motor_rb_en 7

/************   PS2  **************/
#define PS2_DAT        9 //14
#define PS2_CMD        10  //15
#define PS2_SEL        11  //16
#define PS2_CLK        12  //17G

/************  舵机  ***************/
#define servo0          8
#define servo1         9
#define servo2         10

/***********火焰传感器**************/
#define flame_c_1 24 //s0 最低位
#define flame_c_2 26//s1 中
#define flame_c_3 28 //s2 高
#define flame_c_out 66 //接在A12口
#define flame_d_1 25 //s0
#define flame_d_2 27 //s1
#define flame_d_3 29 //s2
#define flame_d_out 65 //A11吧
#define flame_a_1 64  //a left
#define flame_a_2 63
#define flame_a_3 62
#define flame_a_4 61
#define flame_a_5 60
#define flame_b_1 55  // b right
#define flame_b_2 56
#define flame_b_3 57
#define flame_b_4 58
#define flame_b_5 59

/**************巡线******************/
#define xunxian1 48
#define xunxian2 49
#define xunxian3 2 //弃用
#define xunxian4 51
/*******************颜色传感器********/
#define S0     6   // Please notice the Pin's define
#define S1     5
#define S2     4
#define S3     3
#define OUT    2

/****************超声波******************/
#define trig1Pin 11   //前超声
#define echo1Pin 12
#define trig2Pin 13      //后超声
#define echo2Pin 3

/****************Extra******************/
#define tonePin 52 //蜂鸣器
#define switchPin 53 //校准开关
#define headswitchPin1  A13//触碰开关
#define headswitchPin2  A14//另一个触碰开关

