#include <PS2X_lib.h>
#include "PS2X_lib.h"
#include "motor.h"

#define Light        A0
#define Buzzer        49
#define VELOCITY       140//
#define PS2_DAT        8
#define PS2_CMD        9
#define PS2_SEL        10
#define PS2_CLK        11
#define pressures      false
#define rumble         false
unsigned char land_flag = 0;
unsigned char land_position[2] = {0};
//[0]前后,[1]左右 默认往后 不左右
//land_positon[0]: 0后 1 前 
//land_positon[1]: 0不左不右 1左 3右

#define LAND_LEFT 1
#define LAND_RIGHT 3
#define LAND_MIDDLE 0

PS2X ps2x;


float padangle;

int error = 0;
byte type = 0;
byte vibrate = 0;
void (* resetFunc) (void) = 0;
void speed_porcess(int * fwd_v,int* trn_v);
void pad();

void setup() 
{   
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  type = ps2x.readType();
  switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }
  
  Serial.begin(57600);
  Serial1.begin(115200);
  pinMode(47,OUTPUT);
  pinMode(49,OUTPUT);
  pinMode(Light,OUTPUT);
  
  digitalWrite(Light,LOW);
  
  motor_setup();
}


void loop()
{

  pad();

  if (Serial1.available() > 0){
    char c = Serial1.read();
    Serial.println(c);
    if(c == 'X'){
      digitalWrite(Buzzer,HIGH);
      digitalWrite(Light,HIGH);}
      

      if(c == 'T'){
              digitalWrite(Buzzer,LOW);
      digitalWrite(Light,LOW);}
        
      }
      
    }

int send_lock = 1;
void pad()
{
  if(error == 1)
    {
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
    Serial.println("error1");
    }
  else 
  {   int forward_speed = 0;
      int turn_speed = 0;
      forward_speed = 0;
      turn_speed = 0;
      if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)){
        forward_speed = ps2x.Analog(PSS_LY);
        turn_speed = ps2x.Analog(PSS_LX);
        speed_porcess(&forward_speed,&turn_speed);
        Serial.print(forward_speed + turn_speed);
        Serial.print(" ");
        Serial.println(forward_speed - turn_speed);
        
        delay(50);        
      }
      if(ps2x.ButtonReleased(PSB_L1) || ps2x.ButtonReleased(PSB_R1)){
        motor_stop();
        delay(100);
        Serial.println("stoop");
      }
      if(abs(turn_speed) < 60){
      motor_l_work(-(forward_speed+turn_speed));
      motor_r_work(-(forward_speed-turn_speed));}
      else if (abs(turn_speed) > 90){
        motor_l_work(forward_speed+turn_speed);
      motor_r_work(forward_speed-turn_speed);
      }
      delay(5);
      
      ps2x.read_gamepad(false, vibrate); 
      
           
      if(ps2x.ButtonPressed(PSB_CROSS))  
      {   land_position[0] = 0;
                    digitalWrite(Buzzer,HIGH);
          delay(30);
          digitalWrite(Buzzer,LOW);
      }
      else if(ps2x.ButtonPressed(PSB_TRIANGLE)){
          land_position[0] = 1;
                    digitalWrite(Buzzer,HIGH);
          delay(30);
          digitalWrite(Buzzer,LOW);
      }

      if(ps2x.ButtonPressed(PSB_SQUARE))  
      {   land_position[1] = LAND_LEFT;
                    digitalWrite(Buzzer,HIGH);
          delay(30);
          digitalWrite(Buzzer,LOW);
      }
      else if(ps2x.ButtonPressed(PSB_CIRCLE)){
          land_position[1] = LAND_RIGHT;
                    digitalWrite(Buzzer,HIGH);
          delay(30);
          digitalWrite(Buzzer,LOW);
      }

      if(ps2x.ButtonPressed(PSB_L2))  
      {   int package = land_flag | land_position[0] <<1 |land_position[1] <<3 ;
          Serial.write(package);
          
      
      }

      if(ps2x.ButtonPressed(PSB_R2)){
        static int counter = 0;
        counter++;
                digitalWrite(Buzzer,HIGH);
  
      delay(30);
      digitalWrite(Buzzer,LOW);
        if (counter == 3){
          land_flag = 1;
          Serial.println("send");
          unsigned char package = land_flag | land_position[0] <<1 |land_position[1] <<3 ;
          Serial1.write(package);
          send_lock = 0;
          counter = 0;
          digitalWrite(Buzzer,HIGH);
          delay(180);
          digitalWrite(Buzzer,LOW);
        }
      }

      if(ps2x.ButtonPressed(PSB_L3)){
        static int counterr = 0;
        counterr++;
                  digitalWrite(Buzzer,HIGH);
          delay(30);
          digitalWrite(Buzzer,LOW);
          land_flag = 0;
          land_position[0] = 0;
          land_position[1] = 0;
        if (counterr == 2){
          land_flag = 0;
          land_position[0] = 0;
          land_position[1] = 0;
          unsigned char package = land_flag | land_position[0] <<1 |land_position[1] <<3 ;
          Serial1.write(package);
          Serial.print("clear");
          counterr = 0;
          send_lock = 1;
          digitalWrite(Buzzer,HIGH);
          delay(150);
          digitalWrite(Buzzer,LOW);
        }
        
        
      }
      if(send_lock ==0){
        unsigned char package = land_flag | land_position[0] <<1 |land_position[1] <<3 ;
        Serial1.write('k');
        delay(20);
        Serial.println("send");
        }
      
        
}
}


void speed_porcess(int * fwd_v,int* trn_v){
  *fwd_v = *fwd_v-128;
  *trn_v = *trn_v-128;
  if (*fwd_v > 40)
    *fwd_v = map(*fwd_v,40,128,60,220);
  else if(*fwd_v < -40)
    *fwd_v = map(*fwd_v,-128,-40,-220,-60);
  *fwd_v = -*fwd_v;
   if (*trn_v > 40)
    *trn_v = map(*trn_v,40,128,60,220);
   else if (*trn_v < -40)
    *trn_v = map(*trn_v,-128,-40,-220,-60);

   
}
