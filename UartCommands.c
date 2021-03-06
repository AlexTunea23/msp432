/*
 * UartCommands.c
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
#include"UartCommands.h"
#include"Utils.h"
#include "Sensors.h"
#include "Timers.h"
#include"Accelerometer.h"
#define sendingModule EUSCI_A0_MODULE
#define BUFFER_SIZE  510
volatile int tresholdConfiguration;

int valueX,valueX1;
int valueY,valueY1;
int valueZ,valueZ1;

int valueXg;
int valueYg;
int valueZg;

int received_byte;
char xVal[16];
char yVal[16];
char zVal[16];


volatile uint8_t verifyFlag=0;


struct Buffer
{
   int data[BUFFER_SIZE];
    int newest_index;
    int oldest_index;
};

enum BufferStatus
{
	BUFFER_OK,
	BUFFER_EMPTY,
	BUFFER_FULL
};

volatile struct Buffer rx_buffer = {{}, 0, 0};
enum BufferStatus status1, status2, status3;




enum BufferStatus bufferWrite(volatile struct Buffer *buffer, int byte){
    int next_index = (((buffer->newest_index)+1) % BUFFER_SIZE);

    if (next_index == buffer->oldest_index){
        return BUFFER_FULL;
    }

    buffer->data[buffer->newest_index] = byte;
    buffer->newest_index = next_index;
        return BUFFER_OK;
}

enum BufferStatus bufferRead(volatile struct Buffer *buffer, int *byte){

    if (buffer->newest_index == buffer->oldest_index){
        return BUFFER_EMPTY;
    }

    *byte = buffer->data[buffer->oldest_index];
    buffer->oldest_index = ((buffer->oldest_index+1) % BUFFER_SIZE);
        return BUFFER_OK;
}





typedef struct Uarts
{
	uint32_t uartModuleBaseAdress;
	uint8_t uartPort;
	uint8_t pin1Uart;
	uint8_t pin2Uart;
	uint8_t pin3Uart;
	eUSCI_UART_Config uartConfiguration;
	uint8_t primaryModuleFunction;

}Uart;

Uart defaultUartModule;


Uart UartModule0=
{
		.uartModuleBaseAdress=EUSCI_A0_MODULE,
		.uartPort=GPIO_PORT_P1,
		.pin1Uart=GPIO_PIN1,
		.pin2Uart=GPIO_PIN2,
		.pin3Uart=GPIO_PIN3,
		.uartConfiguration=
		{
				EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
		        6,                                     // BRDIV = 78////hjjhihouiykiu7ku
		        8,                                       // UCxBRF = 2
		        32,                                       // UCxBRS = 0
		        EUSCI_A_UART_NO_PARITY,                  // No Parity
		        EUSCI_A_UART_LSB_FIRST,                  // MSB First
		        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
		        EUSCI_A_UART_MODE,                       // UART mode
		        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
		},
		.primaryModuleFunction=GPIO_PRIMARY_MODULE_FUNCTION
};

typedef enum
{
	xAccel,
	yAccel,
	zAccel
}UartSend;





void InitUarts(UartMoudule name)
{
	switch(name)
	{
	case uartModule0:
		defaultUartModule=UartModule0;
		InitUart(defaultUartModule.uartModuleBaseAdress,defaultUartModule.uartPort,defaultUartModule.pin1Uart
				,defaultUartModule.pin2Uart,defaultUartModule.pin3Uart,defaultUartModule.uartConfiguration,
				defaultUartModule.primaryModuleFunction);
		break;
	default:
		break;
	}
}

void InitUartReceive()
{


	  CSKEY = 0x695A;                        // Unlock CS module for register access
	  CSCTL0 = 0;                            // Reset tuning parameters
	  CSCTL0 = DCORSEL_3;                   // Set DCO to 12MHz (nominal, center of 8-16MHz range)
	                                         // Select ACLK = REFO, SMCLK = MCLK = DCO
	  CSCTL1 = SELA_2 | SELS_3 | SELM_3;
	  CSKEY = 0;                             // Lock CS module from unintended accesses

	  // Configure UART pins
	  P1SEL0 |= BIT2 | BIT3;                  // set 2-UART pin as second function

	  __enable_interrupt();
	  NVIC_ISER0 = 1 << ((INT_EUSCIA0 - 16) & 31); // Enable eUSCIA0 interrupt in NVIC module

	  // Configure UART
	  UCA0CTLW0 |= UCSWRST;
	  UCA0CTLW0 |= UCSSEL__SMCLK;             // Put eUSCI in reset
	  // Baud Rate calculation
	  // 12000000/(16*9600) = 78.125
	  // Fractional portion = 0.125
	  // User's Guide Table 21-4: UCBRSx = 0x10
	  // UCBRFx = int ( (78.125-78)*16) = 2
	  UCA0BR0 = 6;                           // 12000000/16/9600
	  UCA0BR1 = 0x00;
	  UCA0MCTLW = 0x1100 | UCOS16 | 0x0080;

	  UCA0CTLW0 &= ~UCSWRST;                  // Initialize eUSCI
	  UCA0IE |= UCRXIE;                       // Enable USCI_A0 RX interrupt
}

void SendCharacterData(char c)
{
	defaultUartModule=UartModule0;
	SendCharacter(defaultUartModule.uartModuleBaseAdress,c);
}

void SendAccelData(char *s)
{
	defaultUartModule=UartModule0;
	SendString(defaultUartModule.uartModuleBaseAdress,s);
}

void UartCommands(char comm[30])
{

	uint32_t periodConfigurationA1,periodConfigurationA0;
	static char *command;
	static char *treshold;
	static char *setValueA1,*setValueA0;
	command=strtok(comm,"=");

	if(strcmp(command,"start")==0)
	{
		//InitTimers();
		StartTimerA1_0();
 		verifyFlag=1;
	}
	else if(strcmp(command,"stop")==0)
	{
		verifyFlag=0;
		//StopTimerA0_0();
		//StopTimerA1_0();
		SendAccelData("OK");
	}
	else if(strcmp(command,"setPeriodA1")==0)
	{
		if(verifyFlag==0)
		{
			setValueA1=strtok(NULL,"}");
			periodConfigurationA1=atoi(setValueA1);
			periodConfigurationA1=(periodConfigurationA1/30);
			SetTimerA1_0Period(periodConfigurationA1);
			SendAccelData("OK");
		}
		else
		{
			SendAccelData("Cannot set period until system is STOP running!");
		}
	}
	else if(strcmp(command,"setPeriodA0")==0)
		{
			if(verifyFlag==0)
			{
				setValueA0=strtok(NULL,"}");
				periodConfigurationA0=atoi(setValueA0);
				periodConfigurationA0=(periodConfigurationA0/30);
				SetTimerA0_0Period(periodConfigurationA0);
				SendAccelData("OK");
			}
			else
			{
				SendAccelData("Cannot set period until system is STOP running!");
			}
		}
	else if(strcmp(command,"setTreshold")==0)
	{
		if(verifyFlag==0)
		{
			treshold=strtok(NULL,"}");
			tresholdConfiguration=atoi(treshold);
			SendAccelData("OK");
		}
		else
		{
			SendAccelData("Cannot set treshold until system is STOP running!");
		}
	}
	else
	{

	}
}


void Add()
{
			valueX=ReadSensors(xAxis);
			valueY=ReadSensors(yAxis);
			valueZ=ReadSensors(zAxis);
			bufferWrite(&rx_buffer, valueX);
			bufferWrite(&rx_buffer, valueY);
			bufferWrite(&rx_buffer, valueZ);
}

void Send()
{
	status1 = bufferRead(&rx_buffer, &received_byte);
	strcpy(xVal, IntToString(received_byte));
	status2 = bufferRead(&rx_buffer, &received_byte);
	strcpy(yVal, IntToString(received_byte));
	status3 = bufferRead(&rx_buffer, &received_byte);
	strcpy(zVal, IntToString(received_byte));

	if(status1==status2==status3==BUFFER_OK)
	{
			SendAccelData("$");
			SendAccelData(xVal);
			SendAccelData(",");
			SendAccelData(yVal);
			SendAccelData(",");
			SendAccelData(zVal);
			SendAccelData("#");
			SendCharacterData(10);
			SendCharacterData(13);
	}
}

uint32_t GetTreshold()
{
	return tresholdConfiguration;
}

uint8_t WaitUntilStart(int treshold)
{
	uint8_t flag=0;
	uint8_t flag1=0;

	if(flag1==0)
	{
	valueX1=ReadSensors(xAxis);
	valueY1=ReadSensors(yAxis);
	valueZ1=ReadSensors(zAxis);
	flag1=1;
	}
	valueX=ReadSensors(xAxis);
	valueY=ReadSensors(yAxis);
	valueZ=ReadSensors(zAxis);
	if(valueX>valueX1+treshold | valueX<valueX1-treshold | valueY>valueY1+treshold | valueY<valueY1-treshold| valueZ>valueZ1+treshold | valueZ<valueZ1-treshold )
		{
			flag=1;
		}
	return flag;
}


uint8_t StopFlag()
{
	return verifyFlag;
}
