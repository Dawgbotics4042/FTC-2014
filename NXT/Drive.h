#include "SAAS/Motors.h"
//#include "gyro.h"
#include "Servos.h"
#include "findSinCos.h"

const tSensors MOTOR_PORT = S1;

const float ROT_RATIO = 1;

/*const float TRIG[] = {	-.7071067812,	// same as SINS[0], SINS[1], COSS[1], and COSS[2]
.7071067812};	// same as SINS[2], SINS[3], COSS[0], and COSS[3]
indices of both SINS[] and COSS[] have identical values to the contained indices
*/

const float SINS[] = {-1 * sinDegrees(45),
	-1 * sinDegrees(135),
	-1 * sinDegrees(225),
	-1 * sinDegrees(315)};

const float COSS[] = {cosDegrees(45),
	cosDegrees(135),
	cosDegrees(225),
	cosDegrees(315)};

typedef struct {
	byte xComp, yComp, rot;
} movData;

void _mtrLeftFront(byte speed)
{
	Motors_SetSpeed(MOTOR_PORT, 2, 1, speed);
}

void _mtrLeftBack(byte speed)
{
	Motors_SetSpeed(MOTOR_PORT, 2, 2, -1 * speed);
}

void _mtrRightFront(byte speed)
{
	Motors_SetSpeed(MOTOR_PORT, 3, 2, speed);
}

void _mtrRightBack(byte speed)
{
	Motors_SetSpeed(MOTOR_PORT, 3, 1, speed);
}

//finds the max of four values
float _max(float a, float b, float c, float d)
{
	a = abs(a);
	b = abs(b);
	c = abs(c);
	d = abs(d);
	if (a < d && b < d && c < d)
		return d;
	if (a < c && b < c && d < c)
		return c;
	if (a < b && d < b && c < b)
		return b;
	return a;
}

/*int useGyro(movData & mov, int oldGyro, int curGyro, int offset)
{
	movData movOut;

	//nxtDisplayBigTextLine(2, "%d", curGyro);
  //nxtDisplayBigTextLine(4, "%d", oldGyro);

  int rot;
	if(mov.rot == 0){
		if (time1[T1] < 300)
			oldGyro = curGyro;
		if(abs(curGyro - oldGyro) < 180)
  	  rot = (int) (1.5 * ((-127.0/180.0) * (curGyro - oldGyro)));
		else
			rot = (int) (1.5 * ((127.0/180.0) * ( (360 - (curGyro - oldGyro)) % 360) ));
	}
  else {
		oldGyro = curGyro;
		rot = mov.rot;
		ClearTimer(T1);
	}

	movOut.rot = (rot < -127 ? -127 : rot);
	movOut.rot = (rot > 127 ? 127 : rot);

	float c = cosDegrees(curGyro-offset); // findCos(;
	float s = sinDegrees(curGyro-offset); // findSin(

	movOut.xComp = (byte) (mov.xComp * c + mov.yComp * s);
	movOut.yComp = (byte) (mov.xComp * -1 * s + mov.yComp * c);

	mov.xComp = movOut.xComp;
	mov.yComp = movOut.yComp;
	mov.rot = movOut.rot;

	return oldGyro;
}
*/

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
void drive(movData mov, byte speed)
{
	float speedWheel[4];
	for (byte n = 0; n<4; n+=1) {
		speedWheel[n] = (mov.xComp * SINS[n] + mov.yComp * COSS[n] + ROT_RATIO * mov.rot);
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

void drive(movData mov, byte speed, long ticks, int offset)
{
	//initGyro(S2);
	//int curGyro;
	//int oldGyro;

	//oldGyro = getGyroData(S2);

	long mot1 = Motors_GetPosition(MOTOR_PORT, 2, 1);
	long mot2 = Motors_GetPosition(MOTOR_PORT, 3, 2);
	long mot3 = Motors_GetPosition(MOTOR_PORT, 2, 2);
	long mot4 = Motors_GetPosition(MOTOR_PORT, 3, 1);

	drive(mov, speed);
	while (true) {
		if (ticks < abs(Motors_GetPosition(MOTOR_PORT, 2, 1) - mot1))
			break;
		if (ticks < abs(Motors_GetPosition(MOTOR_PORT, 3, 2) - mot2))
			break;
		if (ticks < abs(Motors_GetPosition(MOTOR_PORT, 2, 2) - mot3))
			break;
		if (ticks < abs(Motors_GetPosition(MOTOR_PORT, 3, 1) - mot4))
			break;
		//curGyro = getGyroData(S2);
		//useGyro(mov, oldGyro, curGyro, offset);

		drive(mov, speed);
	}
	drive(mov, 0);
}
