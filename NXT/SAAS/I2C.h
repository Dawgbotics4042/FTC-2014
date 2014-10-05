/**
 * @addtogroup NXT
 * Robotc code for the NXT
 * @{
 */

/**
 * @file I2C.h
 * @brief Code to run hitechnic controllers over I2C.
 *
 * IMPORTANT:
 *
 * A. THIS CODE REQUIRES ENCODERS!
 *
 * B. Standard method argument order is:
 *
 * void foo(tSensors port, int daisychainLevel, int motorNumber, sbyte bar)
 *
 * Where foo is your method name and bar is any variable that you need in addition to the standard three.
 * Bar is optional, but your method must always respect motorNumber, port and daisychainLevel.
*/

#include "drivers\common.h"

/**
 * Enables/Disables the PMW output of the servo controller.
 * This is necessary because of the 10 second timeout of the controller.
 *
 * @param port The port on the NXT Ex: S1,S2...
 * @param DaisyChainLevel The daisy chain level of the controller wanted
 * First controller is 1, not 0.
 * @param Status What to set the PMW status to. 0 is on, 170 is on with no timeout, and 255 is off.
 */
void I2C_WritePMW(tSensors port, int DaisyChainLevel, byte Status)
{
    tByteArray I2Crequest;
    tByteArray I2Cresponse;
    I2Crequest[0] = 3;
    I2Crequest[1] = 0x02 * DaisyChainLevel;
    I2Crequest[2] = 0x48;
    I2Crequest[3] = Status;
    writeI2C(port, I2Crequest, I2Cresponse, 1);
}

/**
 * Moves a servo.
 *
 * @param port The port on the NXT. Ex: S1,S2...
 * @param DaisyChainLevel The daisy chain level of the controller wanted.
 * First controller is 1, not 0.
 * @param ServoNumber Choose servo number 1 or 2 on the controller.
 * @param Position The position to set the servo to. From 0-255.
 * It is best to avoid the extremes of that range to avoid hurting the servos.
 */
void I2C_MoveServo(tSensors port, int DaisyChainLevel, int ServoNumber, byte Position)
{
    tByteArray I2Crequest;
    I2Crequest[0] = 3;
    I2Crequest[1] = 0x02 * DaisyChainLevel;
    I2Crequest[2] = 0x41 + ServoNumber;
    I2Crequest[3] = Position;

    writeI2C(port, I2Crequest);
}

/**
 * Sets the mode of the given motor. See
 * http://www.patronumbots.com/uploads/1/3/1/6/13161577/hitechnic_motor_controller_-_brief_v1.3.pdf
 * for details.
 *
 * @param port The port on the NXT. Ex: S1,S2...
 * @param DaisyChainLevel The daisy chain level of the controller wanted.
 * First controller is 1, not 0.
 * @param MotorNumber Choose motor number 1 or 2 on the controller.
 * @param Mode The mode of the motor. See section "Channel mode" on page
 * 3 of above document for details.
 */
void I2C_SetMode(tSensors port, int DaisyChainLevel, int MotorNumber, byte Mode)
{
    if (MotorNumber == 1) {
        tByteArray I2Crequest;
        I2Crequest[0] = 3;
        I2Crequest[1] = 0x02 * DaisyChainLevel;
        I2Crequest[2] = 0x44;
        I2Crequest[3] = Mode;
        writeI2C(port, I2Crequest);
    } else if (MotorNumber == 2) {
        tByteArray I2Crequest;
        I2Crequest[0] = 3;
        I2Crequest[1] = 0x02 * DaisyChainLevel;
        I2Crequest[2] = 0x47;
        I2Crequest[3] = Mode;
        writeI2C(port, I2Crequest);
    }
}

/**
 * Sets the speed of a motor
 *
 * @param port The port on the NXT. Ex: S1,S2...
 * @param DaisyChainLevel The daisy chain level of the controller wanted.
 * First controller is 1, not 0.
 * @param MotorNumber Choose motor number 1 or 2 on the controller.
 * @param Speed The wanted speed of the motor. From -100 to 100.
 */
void I2C_SetMotorSpeed(tSensors port, int daisychainLevel, int MotorNumber, sbyte Speed)
{
    tByteArray I2Crequest;

    I2Crequest[0] = 3;

    // daisychain level 0 will add 0, daisychain level 1 will add 2 to get 0x04, etc.
    I2Crequest[1] = 0x02 * daisychainLevel;

    if(MotorNumber == 1) {
        I2Crequest[2] = 0x45;
        //if we're on motor 1 mode comes first...
        //I2Crequest[3] = 0b00010001;
        I2Crequest[3] = Speed;
    } else {
        I2Crequest[2] = 0x46;
        // ...but if we're on motor 2, speed comes first.
        I2Crequest[3] = Speed;
        //I2Crequest[4] = 0b00010001;
    }

    writeI2C(port, I2Crequest);
}

/**
 * Get the encoder value for a given motor.
 *
 * @param port The port on the NXT. Ex: S1,S2...
 * @param DaisyChainLevel The daisy chain level of the controller wanted.
 * First controller is 1, not 0.
 * @param MotorNumber Choose motor number 1 or 2 on the controller.
 * @return The encoder value of the motor.
 */
long I2C_GetEncoderPosition(tSensors port, int daisychainLevel, int MotorNumber)
{
    daisychainLevel--;
    //initializes the arrays
    tByteArray I2Crequest;
    tByteArray I2Cresponse;

    //sets the number of bytes to send:
    I2Crequest[0] = 2;

    //sends the adress as the first byte
    //daisychain level 0 will add 0, daisychain level 1 will add 2 to get 0x04, etc.
    I2Crequest[1] = 0x02 + daisychainLevel*2;

    //sets the starting position to start sending data at
    if (MotorNumber == 1) {
        I2Crequest[2] = 0x4C;
    } else {
        I2Crequest[2] = 0x50;
    }

    //writes the data, and gets the response
    writeI2C(port, I2Crequest, I2Cresponse, 4);
    //writeDebugStreamLine("%i", I2Cresponse[1]);
    //creates a long out of the bytes
    //note: when debugging with any %i construct, this will be cast to an integer and will overflow at 32767!
    long EncoderValue = (I2Cresponse[0] << 24) + (I2Cresponse[1] << 16) + (I2Cresponse[2] << 8) + (I2Cresponse[3] << 0);

    //returns the long
    return EncoderValue;
    //return ConvertBytesToLong(I2Cresponse[0], I2Cresponse[1], I2Cresponse[2], I2Cresponse[3]);
}

/**
 * Move motor to specified encoder value.
 *
 * @param port The port on the NXT. Ex: S1,S2...
 * @param DaisyChainLevel The daisy chain level of the controller wanted.
 * First controller is 1, not 0.
 * @param MotorNumber Choose motor number 1 or 2 on the controller.
 * @param EncoderPosition The desired motor encoder value.
 * @param MotorSpeed The speed the motor will run to get there. Between -100 and 100.
 */
void I2C_SetEncoderPosition(tSensors port, int daisychainLevel, int MotorNumber, long EncoderPosition, byte MotorSpeed)
{
    if (MotorNumber == 1) {
        tByteArray I2Crequest;
        I2Crequest[0] = 6;
        I2Crequest[1] = 0x02 * daisychainLevel;
        I2Crequest[2] = 0x40;
        I2Crequest[3] = (byte)((EncoderPosition >> 24) & 0x000000ff);
        I2Crequest[4] = (byte)((EncoderPosition >> 16) & 0x000000ff);
        I2Crequest[5] = (byte)((EncoderPosition >> 8) & 0x000000ff);
        I2Crequest[6] = (byte)(EncoderPosition & 0x000000ff);
        writeI2C(port, I2Crequest);

        tByteArray I2Crequest2;
        I2Crequest2[0] = 3;
        I2Crequest2[1] = 0x02 * daisychainLevel;
        I2Crequest2[2] = 0x45;
        I2Crequest2[3] = MotorSpeed;
        writeI2C(port, I2Crequest2);
    } else if (MotorNumber == 2) {
        tByteArray I2Crequest;
        I2Crequest[0] = 3;
        I2Crequest[1] = 0x02 * daisychainLevel;
        I2Crequest[2] = 0x46;
        I2Crequest[3] = MotorSpeed;
        writeI2C(port, I2Crequest);

        tByteArray I2Crequest2;
        I2Crequest2[0] = 6;
        I2Crequest2[1] = 0x02 * daisychainLevel;
        I2Crequest2[2] = 0x48;
        I2Crequest2[3] = (byte)((EncoderPosition >> 24) & 0x000000ff);
        I2Crequest2[4] = (byte)((EncoderPosition >> 16) & 0x000000ff);
        I2Crequest2[5] = (byte)((EncoderPosition >> 8) & 0x000000ff);
        I2Crequest2[6] = (byte)(EncoderPosition & 0x000000ff);
        writeI2C(port, I2Crequest2);
    }
}

/**
 * @}
 */
