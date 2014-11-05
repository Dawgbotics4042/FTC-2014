#define SCL_PIN 2
#define SCL_PORT PORTC
#define SDA_PIN 1
#define SDA_PORT PORTC

#include <SoftI2CMaster.h>

#include "L3G.h"
#include <Wire.h>

L3G Gyro;

void setup()
{
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.begin(9600);
  Wire.begin();
  i2c_init();
  if (!Gyro.init()){
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  //Gyro.enableDefault();
  i2c_start(0xD6);
  i2c_write(L3G_CTRL_REG1);
  i2c_write(0x0F);
  i2c_stop();
  establishContact();
}

byte readReg(byte reg)
{
  byte value;

  //Wire.beginTransmission(0xD6);
  if (!i2c_start(0xD6)) return (byte)-1;
  //Wire.write(reg);
  if (!i2c_write(reg)) return (byte)-1;
 // Wire.endTransmission();
  i2c_stop();
  //Wire.requestFrom(0xD7, (byte)1);
  if (!i2c_start(0xD7)) return (byte)-1;
  //value = Wire.read();
  value = i2c_read(false);
  //Wire.endTransmission();
  i2c_stop();

  return value;
}

void loop()
{
  //Gyro.read();
  byte testX = readReg(0b0101001);//Gyro.g.x;
  byte testY = readReg(0b0101011);//Gyro.g.y;
  if (testX == -1 || testY == -1) {
    Serial.print("ERROR");
  }
  if (Serial.available() > 0) {
    Serial.print("X ");
    Serial.print(testX);
    Serial.print("  Y ");
    Serial.println(testY);
    delay(1000);
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

