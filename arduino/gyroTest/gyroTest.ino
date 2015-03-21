/**
 * @addtogroup arduino
 * @{
 * @addtogroup gyroTest
 * \brief This is the code that works with the gyro, and sends values to the NXT.
 * @{
 */
#include "L3G.h"
#include <Wire.h>

/*void printDouble( double val, unsigned int precision){
    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    Serial.println(frac,DEC);
}*/

L3G Gyro;
double rotation;
unsigned long start_time;
int offset;

void setup()
{
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(13, OUTPUT);

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);

    //Serial.begin(9600);
    Wire.begin();
    if (!Gyro.init()){
    //  Serial.println("Failed to autodetect gyro type!");
        while (1);
    }

    Gyro.enableDefault();
    //establishContact();
    rotation = 0;
    start_time = 0;

    double sum;
    signed char tmp;
    delay(50);

    for (int i = 0; i<1000; i++) {
      tmp = ~Gyro.readReg(0b0101101);
      sum+=tmp;
    }

    offset = round(sum/1000.0);
    delay(2000);
}

void loop()
{
    double rotX = (signed char)~Gyro.readReg(0b0101101) - offset;//Gyro.g.x; 0101001 0b0101101
    rotation+= (rotX) * (millis()-start_time) * (255.0/2000.0) * (18.0/1000.0) * (360.0/347);
    start_time = millis();
    while (rotation < 0)
        rotation = 359 + rotation;
    while (rotation > 359)
        rotation = rotation - 359;
    //printDouble(rotation, 10);
    outputNumber(rotation);
    //Serial.println(rotation);
    //digitalWrite(2, HIGH);
    //digitalWrite(3, HIGH);
    //digitalWrite(4, HIGH);
    //digitalWrite(5, HIGH);
    //digitalWrite(6, HIGH);
    //digitalWrite(7, HIGH);
    //digitalWrite(8, HIGH);
    //digitalWrite(9, HIGH);
    //digitalWrite(10, HIGH);
    //delay(1000);
    //digitalWrite(10, LOW);
    //delay(1000);
    digitalWrite(13, HIGH);
}

/**
 * Connects to the computer with a serial connection.
 * Function blocks until connection is formed.
 */
void establishContact() {
    while (Serial.available() <= 0) {
        Serial.print('A');   // send a capital A
        delay(300);
    }
}

/**
 * Sends the rotation value to the prototype board,
 * and eventually the NXT.
 */
void outputNumber(double rot) {
    short num = (short) round(rot);
    if (num & 1)
        digitalWrite(2, HIGH);
    else
        digitalWrite(2, LOW);
    for (int i = 3; i < 11; i++) {
        if ((num >> (i-2)) & 1)
            digitalWrite(13-i, HIGH);
        else
            digitalWrite(13-i, LOW);
    }
}

/**
 * @}
 * @}
 */
