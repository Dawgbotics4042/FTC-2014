
#include "htpb.h"
void initGyro(tSensors port)
{
	HTPBsetupIO(port, 0);
}

int getGyroData(tSensors port)
{
	ubyte dig =  HTPBreadIO(port, 0xFF);

	int rawAng = HTPBreadADC(port, 0, 8);
	ubyte ang;

	if ((rawAng >> 6 & 1) || (rawAng >> 7 & 1))
		ang = 1;
	else
		ang = 0;

	return (dig << 1) | ang;

}
