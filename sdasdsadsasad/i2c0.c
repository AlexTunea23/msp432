z/*
 * i2c0.c
 *
 *  Created on: Mar 17, 2016
 *      Author: PetruAlexandru
 */

#include"i2c0.h"
#include<driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include<stdio.h>

//void InitI2c0(uint32_t moduleBaseAdress,const eUSCI_I2C_MasterConfig i2cConfig,uint8_t i2cPort,uint8_t sdaPin,
	//	uint8_t sclPin,uint8_t primaryModuleFunction,uint8_t slaveAdress)
//{
	//	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(i2cPort,
	//		        	 sdaPin + sclPin, primaryModuleFunction);
		/* Initializing I2C Master to SMCLK at 100kbs with no autostop */
	//	MAP_I2C_initMaster(moduleBaseAdress, &i2cConfig);
	//	MAP_I2C_setSlaveAddress(moduleBaseAdress,slaveAdress);
		//	    /* Set Master in transmit mode */
	//	MAP_I2C_setMode(moduleBaseAdress, EUSCI_B_I2C_TRANSMIT_MODE);
		//	    /* Enable I2C Module to start operations */
	//	MAP_I2C_enableModule(moduleBaseAdress);
//}

/*void SendI2c0(uint32_t moduleBaseAdress, uint8_t registerAdress, uint8_t registerData)
{
	MAP_I2C_masterSendMultiByteStart(moduleBaseAdress, registerAdress);
		_delay_cycles(100);///send
		MAP_I2C_masterSendMultiByteNext(moduleBaseAdress, registerData);
		MAP_I2C_masterSendMultiByteStop(moduleBaseAdress);
}*/

/*uint8_t ReadI2c0(uint32_t moduleBaseAdress,uint8_t registerAdress)
{
	static uint8_t dataReceived;
	while (MAP_I2C_masterIsStopSent(moduleBaseAdress) == EUSCI_B_I2C_SENDING_STOP);
	I2C_masterSendSingleByte(moduleBaseAdress, registerAdress);
	while (MAP_I2C_masterIsStopSent(moduleBaseAdress) == EUSCI_B_I2C_SENDING_STOP);
	dataReceived=I2C_masterReceiveSingleByte(moduleBaseAdress);
	return dataReceived;
}*/


void InitI2c0(uint32_t moduleBaseAdress,const eUSCI_I2C_MasterConfig i2cConfig,uint8_t i2cPort,uint8_t sdaPin,
		uint8_t sclPin,uint8_t primaryModuleFunction,uint8_t slaveAdress)
{
		MAP_GPIO_setAsPeripheralModuleFunctionInputPin(i2cPort,
			        	 sdaPin + sclPin, primaryModuleFunction);
		/* Initializing I2C Master to SMCLK at 100kbs with no autostop */
		MAP_I2C_initMaster(moduleBaseAdress, &i2cConfig);

		MAP_I2C_setSlaveAddress(moduleBaseAdress,slaveAdress);
			    /* Set Master in transmit mode */
		MAP_I2C_setMode(moduleBaseAdress, EUSCI_B_I2C_TRANSMIT_MODE);
			    /* Enable I2C Module to start operations */
		MAP_I2C_enableModule(moduleBaseAdress);
}


void SendI2c0(uint32_t moduleBaseAdress, uint8_t registerAdress, uint8_t registerData)
{
	MAP_I2C_masterSendMultiByteStart(moduleBaseAdress, registerAdress);
	_delay_cycles(100);///send
	MAP_I2C_masterSendMultiByteNext(moduleBaseAdress, registerData);
	MAP_I2C_masterSendMultiByteStop(moduleBaseAdress);
}


uint8_t ReadI2c0(uint32_t moduleBaseAdress,uint8_t registerAdress)
{
	static uint8_t dataReceived;
	while (MAP_I2C_masterIsStopSent(moduleBaseAdress) == EUSCI_B_I2C_SENDING_STOP);
	I2C_masterSendSingleByte(moduleBaseAdress, registerAdress);
	while (MAP_I2C_masterIsStopSent(moduleBaseAdress) == EUSCI_B_I2C_SENDING_STOP);
	dataReceived=I2C_masterReceiveSingleByte(moduleBaseAdress);

	return dataReceived;
}








