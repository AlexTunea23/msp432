/*
 * Uart0.c
 *
 *  Created on: Mar 11, 2016
 *      Author: PetruAlexandru
 */
#include"Uart0.h"
#include "driverlib.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Sensors.h"
#include"UartCommands.h"
#include"Utils.h"




void InitUart(uint32_t uartModuleBaseAdress,uint8_t uartPort,uint8_t pin1Uart,uint8_t pin2Uart,uint8_t pin3Uart,
		eUSCI_UART_Config uartConfiguration,uint8_t primaryModuleFunction)
{
	/* Selecting P1.2 and P1.3 in UART mode */
			    GPIO_setAsPeripheralModuleFunctionInputPin(uartPort,
			    		pin1Uart | pin2Uart | pin3Uart, primaryModuleFunction);
			    /* Setting DCO to 12MHz */
			    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
			    /* Configuring UART Module */
			    UART_initModule(uartModuleBaseAdress, &uartConfiguration);
			    /* Enable UART module */
			    UART_enableModule(uartModuleBaseAdress);
}




void SendCharacter(uint32_t uartModuleBaseAdress,char characther)
{
	UART_transmitData(uartModuleBaseAdress,characther);
}


void SendString(uint32_t uartModuleBaseAdress,char* myString)
{
	while(*myString!=0)
	     {
	    	UART_transmitData(uartModuleBaseAdress,*myString);
	    	myString++;
	     }
}

void SetPeriod(uint32_t period)
{

}

void SendUartTest(char *a, char *b, char *c)
{
	SendAccelData("x");
	SendAccelData(";");
	SendAccelData("y");
	SendAccelData(";");
	SendAccelData("z");
	SendAccelData(";");
	SendAccelData("a");
	SendAccelData("@");
	SendAccelData(a);
	SendAccelData(",");
	SendAccelData(b);
	SendAccelData(",");
	SendAccelData(c);
	SendAccelData(",");
	SendAccelData("40");
	//SendAccelData(",");

	SendCharacterData(10);
	SendCharacterData(13);
}









