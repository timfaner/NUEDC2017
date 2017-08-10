#define PIN_1 3
#define PIN_2 4
#define PIN_3 5

#define LAND_LEFT 1
#define LAND_RIGHT 3
#define LAND_MIDDLE 0


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(PIN_1,OUTPUT);
pinMode(PIN_2,OUTPUT);
pinMode(PIN_3,OUTPUT);
pinMode(2,OUTPUT);
digitalWrite(2,HIGH);
}
unsigned char land_flag = 0;
unsigned char land_position[2] = {0};
void loop() {
  if(Serial.available()>0){
    char  c = Serial.read();
    land_position[1] = (c>>4) & 3;
    land_position[0] = (c>>2) & 3;
    land_flag = (c) & 3;

    if (land_position[1] == LAND_LEFT){
      digitalWrite(PIN_3,HIGH);
    }
    else if (land_position[1] == 0){
      digitalWrite(PIN_3,LOW);
    }

    if (land_position[0] == 1){
      digitalWrite(PIN_2,HIGH);
    }
    else if (land_position[0] == 0){
      digitalWrite(PIN_2,LOW);
    }

    
    if(land_flag == 1){
      digitalWrite(PIN_1,HIGH);
    }
    else if (land_flag == 0){
      digitalWrite(PIN_1,LOW);
    }

    
  }
  // put your main code here, to run repeatedly:

}
