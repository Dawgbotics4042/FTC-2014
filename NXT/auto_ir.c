#pragma config(Hubs,  S4, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CCustom)
#pragma config(Sensor, S2,     Bumper,         sensorTouch)
#pragma config(Sensor, S3,     HTIRS2,         sensorI2CCustom)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          ir,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          bump,          tmotorNXT, PIDControl)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Servo,  srvo_S4_C1_1,    ball,                 tServoStandard)
#pragma config(Servo,  srvo_S4_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_4,    stand,                tServoStandard)
#pragma config(Servo,  srvo_S4_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S4_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.
#include "SAAS/drivers/hitechnic-irseeker-v2.h"
#include "Drive.h"

void initializeRobot()
{
    //initGyro(S2);
    servo[stand] = 20;
    servo[ball] = 18;
}

/*
bool writeOffset(long offset)
{
    TFileHandle hFileHandle;
    TFileIOResult nIoResult;
    int length = 3;
    string name = "data.txt";
    OpenWrite(hFileHandle, nIoResult, name, length);
    if (nIoResult != 0)
        return false;

    WriteLong(hFileHandle, nIoResult, offset);

    Close(hFileHandle, nIoResult);
    return true;
}
*/

task main()
{
    initializeRobot();

    waitForStart(); // Wait for the beginning of autonomous phase.

    nMotorEncoder[ir] = 0;
    nMotorEncoderTarget[ir] = 150;
    motor[ir] = 75;

    while(nMotorRunState[ir] != runStateIdle) {}
    motor[ir] = 0;

    //int offset = getGyroData(S2);
    //if (!writeOffset((long)offset))
    //    return;

    int _dirAC = 0;

    movData mov;

    mov.xComp = 127;
    mov.yComp = 0;
    mov.rot = 0;

    drive(mov, 65, 3000, 0); // forward

    // Read the ir signal
    for (int i = 0; i<10; i++) {
        _dirAC = HTIRS2readACDir(HTIRS2);
        if (_dirAC != -1)
            break;// I2C read error occurred
    }

    //formula: y = 1.4336622452056X^-.42970111468155: X = inner radius: y = rot/xComp
    mov.xComp = 0;
    mov.yComp = 127;
    mov.rot = -35;
    wait1Msec(10);

    if (_dirAC != 4)
        drive(mov, 127); // start orbiting

    while (_dirAC != 4) { //_dirAC != 4
        // Read the ir signal
        drive(mov, 127);
        //_dirAC= HTIRS2readACDir(HTIRS2);
        for (int i = 0; i<10; i++) {
            _dirAC = HTIRS2readACDir(HTIRS2);
            if (_dirAC != -1)
                break;// I2C read error occurred
        }
        if (_dirAC == -1) {
            return;
        }
        //if (_dirAC < 0)
        //    return; // I2C read error occurred
    }
    drive(mov, 0); //stop orbiting

    nMotorEncoder[ir] = 0;
    nMotorEncoderTarget[ir] = 20;
    motor[ir] = 25;

    while(nMotorRunState[ir] != runStateIdle) {}
    motor[ir] = 0;

    mov.xComp = 127;
    mov.yComp = 0;
    mov.rot = 0;

    ClearTimer(T1);

    nMotorEncoder[bump] = 0;
    nMotorEncoderTarget[bump] = 95;
    motor[bump] = 25;

    while((nMotorRunState[bump] != runStateIdle) && (time1[T1] <= 2000)) {}
    motor[bump] = 0;

    while((SensorValue(Bumper) == 0) && (time1[T1] <= 3000))
    {
        drive(mov, 30);
    }
    drive(mov, 0);

    wait1Msec(100);

    mov.xComp = -127;
    mov.yComp = 0;
    mov.rot = 0;
    drive(mov, 30, 250, 0);

    ClearTimer(T1);

    nMotorEncoder[bump] = 0;
    nMotorEncoderTarget[bump] = -95;
    motor[bump] = -25;

    while(nMotorRunState[bump] != runStateIdle && (time1[T1] <= 2000)) {}
    motor[bump] = 0;

    /*mov.xComp = 0;
    mov.yComp = -127;
    mov.rot = 0;

    drive(mov, 127, 100, offset);
    drive(mov, 0);
    */

    //lift up
    float rotations = 28.9;

    long mot1 = I2C_GetEncoderPosition(S1, 1, 1)-1120*rotations;
    long mot2 = I2C_GetEncoderPosition(S1, 1, 2)+1120*rotations;

    Motors_SetPosition(S1, 1, 1, mot1, 100);
    Motors_SetPosition(S1, 1, 2, mot2, 100);

    while (I2C_GetEncoderPosition(S1, 1, 1) > mot1) {}
    while (I2C_GetEncoderPosition(S1, 1, 2) < mot2) {}

    mov.xComp = 0;
    mov.yComp = 0;
    mov.rot = 127;

    //drive(mov, 20, 1250, offset);

    for (int i = 0; i<10; i++) {
        _dirAC = HTIRS2readACDir(HTIRS2);
        if (_dirAC != -1)
            break;// I2C read error occurred
    }

    if (_dirAC != 7)
        drive(mov, 20); // start rotation

    while (_dirAC < 7) { //_dirAC != 4
        drive(mov, 20);
        // Read the ir signal
        _dirAC= HTIRS2readACDir(HTIRS2);
        if (_dirAC < 0)
            break;
             // I2C read error occurred
        //nxtDisplayClearTextLine(2);
    //nxtDisplayBigTextLine(2, "%d", _dirAC);
    }
    drive(mov, 20, 2000, 0);
    drive(mov, 20, 100, 0);
    drive(mov, 0);


    //drop
    servo[ball] = 160;
    while (ServoValue[ball] != 160) {}

    mov.xComp = 0;
    mov.yComp = 127;
    mov.rot = 0;

    drive(mov, 20, 1001, 0);

    Motors_SetSpeed(S1, 1, 1, 0);
    Motors_SetSpeed(S1, 1, 2, 0);
    wait10Msec(100);



    //offset = getGyroData(S2);
    //if (!writeOffset((long)offset))
    //    return;
}
