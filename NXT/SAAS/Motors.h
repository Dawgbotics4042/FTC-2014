#include "I2C.h"

int Motors_Length = 0;

tSensors Motors_Port[10];
int Motors_DaisyChainLevel[10];
int Motors_MotorNumber[10];
byte Motors_Mode[10];


byte Motors_ModeByLocation(tSensors Port, byte DaisyChainLevel, int MotorNumber)
{
	for(int i = 0; i < Motors_Length; i++)
		if(Port == Motors_Port[i] && DaisyChainLevel == Motors_DaisyChainLevel[i] && MotorNumber == Motors_MotorNumber[i])
			return Motors_Mode[i];

	return 0b00000000;
}

int Motors_ContainsMode(tSensors Port, byte DaisyChainLevel, int MotorNumber)
{
	for(int i = 0; i < Motors_Length; i++)
		if(Port == Motors_Port[i] && DaisyChainLevel == Motors_DaisyChainLevel[i] && MotorNumber == Motors_MotorNumber[i])
			return i;

	return -1;
}

void Motors_NewMode(tSensors Port, byte DaisyChainLevel, int MotorNumber, byte Mode)
{
	I2C_SetMode(Port, DaisyChainLevel, MotorNumber, Mode);

	int IndexOfList = Motors_ContainsMode(Port, DaisyChainLevel, MotorNumber);
	if(IndexOfList != -1)
	{
		Motors_Mode[IndexOfList] = Mode;
	}
	else
	{
		Motors_DaisyChainLevel[Motors_Length] = DaisyChainLevel;
		Motors_MotorNumber[Motors_Length] = MotorNumber;
		Motors_Port[Motors_Length] = Port;
		Motors_Mode[Motors_Length] = Mode;
		Motors_Length++;
	}
}

void Motors_SetSpeed(tSensors Port, byte DaisyChainLevel, int MotorNumber, byte inSpeed)
{
	byte Speed = (byte) round((78.0/127.0) * inSpeed);
	//byte Speed = inSpeed;
	if(MotorNumber == 1)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, 0b00000001); //0b00010001
		Motors_NewMode(Port, DaisyChainLevel, 2, Motors_ModeByLocation(Port, DaisyChainLevel, 2) | 0b00000000); //0b00010000

		I2C_SetMotorSpeed(Port, DaisyChainLevel, MotorNumber, Speed);
	}
	else if (MotorNumber == 2)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, Motors_ModeByLocation(Port, DaisyChainLevel, 1) | 0b00000000); //0b00010000
		Motors_NewMode(Port, DaisyChainLevel, 2, 0b00000001); //0b00010001

		I2C_SetMotorSpeed(Port, DaisyChainLevel, MotorNumber, Speed);
	}
}

void Motors_SetPosition(tSensors Port, byte DaisyChainLevel, int MotorNumber, long MotorEncoder, int Speed)
{
	if(MotorNumber == 1)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, 0b00010010);
		Motors_NewMode(Port, DaisyChainLevel, 2, Motors_ModeByLocation(Port, DaisyChainLevel, 2) | 0b00010000);

		I2C_SetEncoderPosition(Port, DaisyChainLevel, MotorNumber, MotorEncoder, Speed);
	}
	else if (MotorNumber == 2)
	{
		Motors_NewMode(Port, DaisyChainLevel, 1, Motors_ModeByLocation(Port, DaisyChainLevel, 1) | 0b00010000);
		Motors_NewMode(Port, DaisyChainLevel, 2, 0b00010010);

		I2C_SetEncoderPosition(Port, DaisyChainLevel, MotorNumber, MotorEncoder, Speed);
	}
}

void Motors_MoveRotations(tSensors Port, byte DaisyChainLevel, int MotorNumber, float Rotations, int Speed)
{
	Motors_SetPosition(Port, DaisyChainLevel, MotorNumber, I2C_GetEncoderPosition(Port, DaisyChainLevel, MotorNumber) + (Rotations * 1120), Speed);
}

int Motors_GetPosition(tSensors Port, byte DaisyChainLevel, int MotorNumber)
{
	return I2C_GetEncoderPosition(Port, DaisyChainLevel, MotorNumber);
}

bool Motors_IsMoving(tSensors Port, byte DaisyChainLevel, int MotorNumber) {
	int currentPos = 0;
	int lastPos = 0;
	currentPos = I2C_GetEncoderPosition(Port, DaisyChainLevel, MotorNumber);
	wait1Msec(1000);
	lastPos = currentPos;
	currentPos = I2C_GetEncoderPosition(Port, DaisyChainLevel, MotorNumber);
	int deriv = currentPos - lastPos;
	if(abs(deriv) <= 100)
		return false;
	else
		return true;
}
