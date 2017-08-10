#include <Arduino.h>

#include "line.h"

//初始化电机，放setup里
void motor_setup() {
  pinMode(motor_lf_in1, OUTPUT);
  pinMode(motor_lf_in2, OUTPUT);
  pinMode(motor_lf_en, OUTPUT);
  pinMode(motor_rf_in1, OUTPUT);
  pinMode(motor_rf_in2, OUTPUT);
  pinMode(motor_rf_en, OUTPUT);
  pinMode(motor_lb_in1, OUTPUT);
  pinMode(motor_lb_in2, OUTPUT);
  pinMode(motor_lb_en, OUTPUT);
  pinMode(motor_rb_in1, OUTPUT);
  pinMode(motor_rb_in2, OUTPUT);
  pinMode(motor_rb_en, OUTPUT);
}


//运转函数,pwm:-255~255,正值正转，负值反转
int motor_lf_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_lf_in1, HIGH);
    digitalWrite(motor_lf_in2, LOW);
    analogWrite(motor_lf_en, pwm);
  }
  else {
    digitalWrite(motor_lf_in1, LOW);
    digitalWrite(motor_lf_in2, HIGH);
    analogWrite(motor_lf_en, -pwm);
  }
}

int motor_rf_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_rf_in1, HIGH);
    digitalWrite(motor_rf_in2, LOW);
    analogWrite(motor_rf_en, pwm);
  }
  else {
    digitalWrite(motor_rf_in1, LOW);
    digitalWrite(motor_rf_in2, HIGH);
    analogWrite(motor_rf_en, -pwm);
  }
}

int motor_lb_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_lb_in1, HIGH);
    digitalWrite(motor_lb_in2, LOW);
    analogWrite(motor_lb_en, pwm);
  }
  else {
    digitalWrite(motor_lb_in1, LOW);
    digitalWrite(motor_lb_in2, HIGH);
    analogWrite(motor_lb_en, -pwm);
  }
}

int motor_rb_work(int pwm) {
  if (pwm >= 0) {
    digitalWrite(motor_rb_in1, HIGH);
    digitalWrite(motor_rb_in2, LOW);
    analogWrite(motor_rb_en, pwm);
  }
  else {
    digitalWrite(motor_rb_in1, LOW);
    digitalWrite(motor_rb_in2, HIGH);
    analogWrite(motor_rb_en, -pwm);
  }
}

void motor_l_work(int pwm){
  pwm = - pwm;
  motor_lb_work(pwm);
  motor_lf_work(pwm);
}

void motor_r_work(int pwm){
  motor_rb_work(pwm);
  motor_rf_work(pwm);
}

//刹车&锁死轮胎
void motor_stop(){
    digitalWrite(motor_lf_in1, HIGH);
    digitalWrite(motor_lf_in2, HIGH);
    analogWrite(motor_lf_en, 255);
    digitalWrite(motor_lb_in1, HIGH);
    digitalWrite(motor_lb_in2, HIGH);
    analogWrite(motor_lb_en, 255);

    digitalWrite(motor_rf_in1, HIGH);
    digitalWrite(motor_rf_in2, HIGH);
    analogWrite(motor_rf_en, 255);
    digitalWrite(motor_rb_in1, HIGH);
    digitalWrite(motor_rb_in2, HIGH);
    analogWrite(motor_rb_en, 255);


}


void motor_test() {
  motor_l_work(255);
  motor_r_work(255);
  delay(1000);
  motor_l_work(150);
  motor_r_work(150);
  delay(1000);
  motor_l_work(-255);
  motor_r_work(-255);
  delay(1000);
  motor_l_work(-150);
  motor_r_work(-150);
  delay(1000);
}

