/*
 * i2c0.h
 *
 *  Created on: Mar 11, 2016
 *      Author: PetruAlexandru
 */
#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_
#include <stdint.h>
#include <string.h>

typedef enum
{
	Mpu,
	Gy80
}Accelerometer;

typedef enum
{
	XAxis,
	YAxis,
	ZAxis,

}AxisAccelerometer;

typedef enum
{
	XAxisG,
	YAxisG,
	ZAxisG
}AxisGyroscope;

typedef enum
{
	Range2G,
	Range4G,
	Range8G,
	Range16G
}RangeAccelerometer;

typedef enum
{
	Temp
}Temperature;

typedef enum
{
	r2G,
	r4G,
	r8G,
	r16G
}Grange;


void InitAccelerometer(Accelerometer Id);

void ConfigAccelerometer(RangeAccelerometer Range);

int32_t ReadAccelerometer(AxisAccelerometer Axis);

int32_t ReadGyroscope(AxisGyroscope AxisG);

int16_t ReadTemperature(Temperature temp);

#endif /* ACCELEROMETER_H_ */
