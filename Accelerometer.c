/*
 * i2c0.h
 *
 *  Created on: Mar 11, 2016
 *      Author: PetruAlexandru
 */
#include "Accelerometer.h"
#include "i2c0.h"
#include<driverlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include"Utils.h"

#define POWER_REGISTER_ADRESS 						0x6B
#define POWER_REGISTER_DATA 						0
#define SLAVE_ADRESS_MPU 							0x68

#define X_LOW_REGISTER_ADRESS_MPU 					0x3C
#define X_HIGH_REGISTER_ADRESS_MPU 					0x3B
#define Y_LOW_REGISTER_ADRESS_MPU 					0x3E
#define Y_HIGH_REGISTER_ADRESS_MPU 					0x3D
#define Z_LOW_REGISTER_ADRESS_MPU 					0x40
#define Z_HIGH_REGISTER_ADRESS_MPU 					0x3F

#define X_LOW_REGISTER_ADRESS_GY  					0x3C
#define X_HIGH_REGISTER_ADRESS_GY   				0x3B
#define Y_LOW_REGISTER_ADRESS_GY  					0x3E
#define Y_HIGH_REGISTER_ADRESS_GY  					0x3D
#define Z_LOW_REGISTER_ADRESS_GY  					0x40
#define Z_HIGH_REGISTER_ADRESS_GY  					0x3F

#define CONFIG_REGISTER_ADRESS_MPU					0x1C
#define RANGE_2G_CONFIG_REGISTER_DATA				0
#define RANGE_4G_CONFIG_REGISTER_DATA				0x08
#define RANGE_8G_CONFIG_REGISTER_DATA				0x10
#define RANGE_16G_CONFIG_REGISTER_DATA				0x18
#define LBS_2G										16384
#define LBS_4G										8192
#define LBS_8G										4096
#define LBS_16G										2048


typedef struct Sensors
{
	uint8_t slaveAdress;
	uint32_t moduleBaseAdress;
	uint8_t powerRegisterAdress;
	uint8_t powerRegisterData;
	eUSCI_I2C_MasterConfig i2cConfiguration;
	uint8_t i2cPort;
	uint8_t sdaPin;
	uint8_t sclPin;
	uint8_t primaryModuleFunction;
	uint8_t xLowAdress;
	uint8_t xHighAdress;
	uint8_t yLowAdress;
	uint8_t yHighAdress;
	uint8_t zLowAdress;
	uint8_t zHighAdress;
	uint8_t configRegisterAdress;
	uint8_t range2G;
	uint8_t range4G;
	uint8_t range8G;
	uint8_t range16G;
	uint32_t lbs2G;
	uint32_t lbs4G;
	uint32_t lbs8G;
	uint32_t lbs16G;

}Sensor;

Sensor sensorMpu=
{		.slaveAdress=SLAVE_ADRESS_MPU,
		.moduleBaseAdress=EUSCI_B0_MODULE,
		.powerRegisterAdress=POWER_REGISTER_ADRESS,
		.powerRegisterData=POWER_REGISTER_DATA,
		.i2cConfiguration=
		{
				EUSCI_B_I2C_CLOCKSOURCE_SMCLK,3000000,
				EUSCI_B_I2C_SET_DATA_RATE_100KBPS,
				EUSCI_B_I2C_NO_AUTO_STOP
		},
		.i2cPort=GPIO_PORT_P1,
		.sdaPin=GPIO_PIN6,
		.sclPin=GPIO_PIN7,
		.primaryModuleFunction=GPIO_PRIMARY_MODULE_FUNCTION,
		.xLowAdress=X_LOW_REGISTER_ADRESS_MPU,
		.xHighAdress=X_HIGH_REGISTER_ADRESS_MPU,
		.yLowAdress=Y_LOW_REGISTER_ADRESS_MPU,
		.yHighAdress=Y_HIGH_REGISTER_ADRESS_MPU,
		.zLowAdress=Z_LOW_REGISTER_ADRESS_MPU,
		.zHighAdress=Z_HIGH_REGISTER_ADRESS_MPU,
		.range2G=RANGE_2G_CONFIG_REGISTER_DATA,
		.range4G=RANGE_4G_CONFIG_REGISTER_DATA,
		.range8G=RANGE_8G_CONFIG_REGISTER_DATA,
		.range16G=RANGE_16G_CONFIG_REGISTER_DATA,
		.lbs2G=LBS_2G,
		.lbs4G=LBS_4G,
		.lbs8G=LBS_8G,
		.lbs16G=LBS_16G
};

Sensor defaultSensor;

void InitAccelerometer(Accelerometer Id)
{

	switch(Id)
	{
	case Mpu:
		defaultSensor=sensorMpu;

		InitI2c0(defaultSensor.moduleBaseAdress,defaultSensor.i2cConfiguration,
				defaultSensor.i2cPort,defaultSensor.sdaPin,defaultSensor.sclPin,
				defaultSensor.primaryModuleFunction,defaultSensor.slaveAdress);


		SendI2c0(defaultSensor.moduleBaseAdress,defaultSensor.powerRegisterAdress,
					defaultSensor.powerRegisterData);
		break;

	case Gy80:
		//Init_I2c(defaultSensor.moduleBaseAdress,defaultSensor.i2cConfiguration,defaultSensor.i2cPort,defaultSensor.sdaPin,defaultSensor.sclPin,defaultSensor.primaryModuleFunction);

		break;
	default:
		break;
	}
}



void ConfigAccelerometer(RangeAccelerometer Range)
{
	switch(Range)
	{
	case Range2G:
		SendI2c0(defaultSensor.moduleBaseAdress,defaultSensor.configRegisterAdress,defaultSensor.range2G);
		break;
	case Range4G:
		SendI2c0(defaultSensor.moduleBaseAdress,defaultSensor.configRegisterAdress,defaultSensor.range4G);
		break;
	case Range8G:
			SendI2c0(defaultSensor.moduleBaseAdress,defaultSensor.configRegisterAdress,defaultSensor.range8G);
			break;
	case Range16G:
			SendI2c0(defaultSensor.moduleBaseAdress,defaultSensor.configRegisterAdress,defaultSensor.range16G);
			break;
	default:
		break;
	}
}


int32_t ReadAccelerometer(AxisAccelerometer Axis)
{
	defaultSensor=sensorMpu;
	static uint8_t dataReceivedLow=0;
	static uint8_t dataReceivedHigh=0;
	static uint32_t dataReceivedHighLow=0;
	static int32_t dataReceived=0;

	switch(Axis)
	{
	case XAxis:
		dataReceivedLow=ReadI2c0(defaultSensor.moduleBaseAdress,defaultSensor.xLowAdress);
		dataReceivedHigh=ReadI2c0(defaultSensor.moduleBaseAdress,defaultSensor.xHighAdress);
		break;
	case YAxis:
		dataReceivedLow=ReadI2c0(defaultSensor.moduleBaseAdress,defaultSensor.yLowAdress);
		dataReceivedHigh=ReadI2c0(defaultSensor.moduleBaseAdress,defaultSensor.yHighAdress);
		break;
	case ZAxis:
		dataReceivedLow=ReadI2c0(defaultSensor.moduleBaseAdress,defaultSensor.zLowAdress);
		dataReceivedHigh=ReadI2c0(defaultSensor.moduleBaseAdress,defaultSensor.zHighAdress);
		break;
	default:
		break;
	}
	dataReceivedHighLow=(dataReceivedHigh<<8)|(dataReceivedLow);
	dataReceived=HexaToDecG(dataReceivedHighLow,defaultSensor.lbs2G);
	return dataReceived;
}




















