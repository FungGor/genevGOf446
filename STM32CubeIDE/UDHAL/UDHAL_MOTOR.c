/*
 * UDHAL_MOTOR.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#include "../UDHAL/UDHAL_MOTOR.h"

#include "mc_api.h"
#include "mc_type.h"
#include "hardwareParam.h"

static uint8_t CURRENT_SAMPLES_INDEX = 0;
float RMS_CURRENT_SAMPLES[CURRENT_SAMPLES];
float RMS_CURRENT_SUM = 0;

void accelerateIQMotor(int16_t torque, uint16_t ramp)
{
	MC_ProgramTorqueRampMotor1(torque,ramp);
	MC_StartMotor1();
}

void accelerateSpeedMotor(int16_t targetSpeed, uint16_t ramp)
{
	MC_ProgramSpeedRampMotor1(targetSpeed, ramp);
	MC_StartMotor1();
}

void motorStop()
{
	MC_StopMotor1();
}

extern void ackErrorDebug()
{
	MC_AcknowledgeFaultMotor1();
}

int16_t getSpeed()
{
	/*Returns the speed in RPM*/
	int16_t MOTOR_SPEED = (MC_GetMecSpeedAverageMotor1() * _RPM) / SPEED_UNIT;
	return MOTOR_SPEED;
}

float getCurrent()
{
	float MOTOR_CURRENT = (MC_GetPhaseCurrentAmplitudeMotor1() * VDD_SUPPLY)/(ADC_RANGE*CURRENT_SENSOR_GAIN);
	return MOTOR_CURRENT;
}

float getRMSCurrent()
{
	float RMS_DC_SAMPLE = (getCurrent()/RMS_FACTOR);
	RMS_CURRENT_SAMPLES[CURRENT_SAMPLES_INDEX] = RMS_DC_SAMPLE;

	uint8_t jj = 0;
	float rmsDCsum = 0;

	for(jj = 0; jj<CURRENT_SAMPLES; jj++)
	{
		rmsDCsum += RMS_CURRENT_SAMPLES[jj];
	}

	float rmsDCAvg = rmsDCsum/CURRENT_SAMPLES;

	CURRENT_SAMPLES_INDEX++;
	if(CURRENT_SAMPLES_INDEX >= CURRENT_SAMPLES)
	{
		CURRENT_SAMPLES_INDEX = 0;
	}
	return rmsDCAvg;
}

int16_t getVoltage()
{
	int16_t MOTOR_VOLTAGE = MC_GetPhaseVoltageAmplitudeMotor1();
	return MOTOR_VOLTAGE;
}

void motorAnalysis()
{

}
