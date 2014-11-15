#include "L3G.h"
#include <Wire.h>

void printDouble( double val, unsigned int precision){
    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    Serial.println(frac,DEC);
} 

L3G Gyro;
double rotation;
unsigned long start_time;
int offset;

void setup()
{
  
  //pinMode(5, OUTPUT);
  //digitalWrite(5, HIGH);
  Serial.begin(9600);
  Wire.begin();
  if (!Gyro.init()){
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  Gyro.enableDefault();
  establishContact();
  rotation = 0;
  start_time = 0;
  
  double sum;
  signed char tmp;
  delay(50);
  
  for (int i = 0; i<1000; i++) {
    tmp = ~Gyro.readReg(0b0101001);
    sum+=tmp;
  }
  
  offset = round(sum/1000.0);
}

void loop()
{
  //if (start_time == 0) {
  //} else {
    double rotX = (signed char)~Gyro.readReg(0b0101001) - offset;//Gyro.g.x;
   /* if (Serial.available() > 0) {
      Serial.print("X ");z
      Serial.println(rotX-offset);
      delay(250);
    }*/
    //unsigned long time_from_last = (millis() - start_time);
    //if (rotX <= 1 || rotX >= -1)
    //  rotX = 0;
    rotation+= (rotX) * (millis()-start_time) * 255.0/2000.0 * 18.0/1000.0;
    start_time = millis();
    /*while (rotation < 0) {
      rotation = 360000-rotation;
    }*/
   /* if (millis() % 1000 == 1) {
      //printDouble(rotation, 5);
      Serial.println(rotation);
      //Serial.println(start_time);
    }
    start_time = millis();*/
    printDouble(rotation, 10);
//  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

