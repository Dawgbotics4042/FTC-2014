#pragma config(Sensor, S1,     ,               sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "SAAS/Motors.h"

task main()
{
	while(true)
	{
    Motors_SetSpeed(S1, 1, 1, -100);
  	Motors_SetSpeed(S1, 1, 2, 100);
  }

}
