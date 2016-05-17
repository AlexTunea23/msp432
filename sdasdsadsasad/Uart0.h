/*
 * Uart0.h
 *
 *  Created on: Mar 11, 2016
 *      Author: PetruAlexandru
 */

#ifndef UART0_H_
#define UART0_H_
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include"driverlib.h"

void InitUart(uint32_t uartModuleBaseAdress,uint8_t uartPort,uint8_t pin1Uart,uint8_t pin2Uart,
		uint8_t pin3Uart,const eUSCI_UART_Config uartConfiguration,uint8_t primaryModuleFunction);

void SendCharacter(uint32_t uartModuleBaseAdress,char characther);

void SendString(uint32_t uartModuleBaseAdress,char* myString);

void SetPeriod(uint32_t period);

void StartAquisition();


#endif /* UART0_H_ */
