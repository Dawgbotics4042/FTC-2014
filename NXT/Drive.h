#include "SAAS/Motors.h"

const tSensors MOTOR_PORT = S1;

const float ROT_RATIO = 1;

/*const float TRIG[] = {	-.7071067812,	// same as SINS[0], SINS[1], COSS[1], and COSS[2]
												.7071067812};	// same as SINS[2], SINS[3], COSS[0], and COSS[3]
				indices of both SINS[] and COSS[] have identical values to the contained indices
*/

const float SINS[] = {	-.7071067812,	//same as--> -1 * sinDegrees(45),
                        -.7071067812,	//same as--> -1 * sinDegrees(135),
                        .7071067812,	//same as--> -1 * sinDegrees(225),
                        .7071067812};	//-1 * sinDegrees(315)};

const float COSS[] = {cosDegrees(45),
                        cosDegrees(135),
                        cosDegrees(225),
                        cosDegrees(315)};

void _mtrLeftFront(byte speed)
{
    Motors_SetSpeed(MOTOR_PORT, 2, 2, speed);
}

void _mtrLeftBack(byte speed)
{
    Motors_SetSpeed(MOTOR_PORT, 2, 1, speed);
}

void _mtrRightFront(byte speed)
{
    Motors_SetSpeed(MOTOR_PORT, 1, 2, speed);
}

void _mtrRightBack(byte speed)
{
    Motors_SetSpeed(MOTOR_PORT, 1, 1, speed);
}

float _max(float a, float b, float c, float d)
{
    if (a < b) {
        if (b < c) {
            if (c < d)
                return d;
            else
                return c;
        } else {
            return b;
        }
    }
    return a;
}

/**
 * Sets the speed for all four motors for a omni dive.
 *
 * @param xComp The x component of the vector of the direction
 * the robot should go.
 * @param yComp The y component of the vector of the direction
 * the robot should go.
 * @param speed The speed the robot should go at. Out of
 * 255.
 * @param rot The rotation speed of the robot.
 */
void drive(byte xComp, byte yComp, byte speed, byte rot)
{
    float speedWheel[4];
    for (byte n = 0; n<4; n+=1) {
        speedWheel[n] = (xComp * SINS[n] + yComp * COSS[n] + ROT_RATIO * rot);
    }

    float scaler = abs(127.0/_max(speedWheel[0], speedWheel[1], speedWheel[2], speedWheel[3]));
    byte speedFinal[4];

    for (byte n = 0; n<4; n+=1) {
        speedFinal[n] = (byte) (speed/127.0 * scaler * speedWheel[n]);
    }

    _mtrRightFront(speedFinal[0]);
    _mtrLeftFront(speedFinal[1]);
    _mtrLeftBack(speedFinal[2]);
    _mtrRightBack(speedFinal[3]);
}
