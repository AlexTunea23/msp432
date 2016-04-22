/*
 * i2c0.h
 *
 *  Created on: Mar 17, 2016
 *      Author: PetruAlexandru
 */

#ifndef I2C0_H_
#define I2C0_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include<driverlib.h>


void InitI2c0(uint32_t moduleBaseAdress,const eUSCI_I2C_MasterConfig i2cConfig,uint8_t i2cPort,
		uint8_t sdaPin, uint8_t sclPin,uint8_t primaryModuleFunction,uint8_t slaveAdress);

void SendI2c0(uint32_t moduleBaseAdress, uint8_t powerRegisterAdress, uint8_t powerRegisterData);

uint8_t ReadI2c0(uint32_t moduleBaseAdress,uint8_t registerAdress);


#endif /* I2C0_H_ */
