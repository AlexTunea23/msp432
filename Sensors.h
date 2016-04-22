/*
 * i2c0.h
 *
 *  Created on: Mar 11, 2016
 *      Author: PetruAlexandru
 */
#ifndef SENSORS_H_
#define SENSORS_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>



typedef enum
{
	mpu,
	gy80
}IdSensors;

typedef enum
{
	xAxis,
	yAxis,
	zAxis
}AxisSensors;

typedef enum
{
	range2G,
	range4G,
	range8G,
	range16G
}RangeConfig;

void InitSensors(IdSensors Name);

void ConfigSensors(RangeConfig range);

int32_t ReadSensors(AxisSensors Axiss);


#endif /* SENSORS_H_ */
