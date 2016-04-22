/*
 * i2c0.h
 *
 *  Created on: Mar 11, 2016
 *      Author: PetruAlexandru
 */
#include "Sensors.h"
#include "Accelerometer.h"
#include <stdint.h>
#include <string.h>
#include<driverlib.h>

void InitSensors(IdSensors Name)
{
	switch(Name)
	{
	case mpu:
		InitAccelerometer(Mpu);
		break;
	default:
		break;
	}
}

void ConfigSensors(RangeConfig range)
{
	switch(range)
	{
	case range2G:
		ConfigAccelerometer(Range2G);
		break;
	case range4G:
		ConfigAccelerometer(Range4G);
		break;
	case range8G:
			ConfigAccelerometer(Range8G);
			break;
	case range16G:
			ConfigAccelerometer(Range16G);
			break;
	default:
		break;
	}
}



int32_t ReadSensors(AxisSensors Axiss)
{
	static int32_t readValue=0;
	switch(Axiss)
	{
	case xAxis:
		readValue=ReadAccelerometer(XAxis);
		break;
	case yAxis:
		readValue=ReadAccelerometer(YAxis);
		break;
	case zAxis:
		readValue=ReadAccelerometer(ZAxis);
	default:
		break;
	}
	return readValue;
}





