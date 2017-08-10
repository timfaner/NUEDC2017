//电机函数

void motor_setup();  //初始化 放在setup里


/******************接下来是电机使用函数**************/

 //pwm是电机转速
 //电机驱动函数 分别为 leftfront(左前lf) rightback(右后rb)，其他自己领会
int motor_lf_work(int pwm);
int motor_rf_work(int pwm);
int motor_lb_work(int pwm);
int motor_rb_work(int pwm);
void motor_l_work(int pwm);
void motor_r_work(int pwm);
void motor_stop();

//测试电机是否接线正确，正确表现为左右轮同时向前快走1s，慢走一秒
//接着同时向后快走一秒，慢走一秒
void motor_test();

