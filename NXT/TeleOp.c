#pragma config(Sensor, S1,     ,               sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Drive.h"
#include "JoystickDriver.c"

 task main()
{
  waitForStart();

  byte movX, movY, rot;

	while( true )
	{
		movX = joystick.joy1_x1;
		movY = joystick.joy1_y1-1;
		rot = -1*(joystick.joy1_x2+1);

		if (rot < 10 && rot > 10)
			rot = 0;
		if (movX < 10 && movX > 10)
			movX = 0;
	  if (movY < 10 && movY > 10)
			movY = 0;

		float speed = sqrt( pow(movX, 2) + pow(movY, 2) ) + abs(rot);
		if (speed > 127) speed = 127;

		speed = floor(speed);
		writeDebugStreamLine("%f: %f", movX, movY);

		drive( movX, movY, (byte)speed, rot );

		//drive(0,127,127,0);

		// 127 = counterclock
	}
}