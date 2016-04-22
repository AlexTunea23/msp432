
#include <driverlib.h>
#include "UartCommands.h"
#include "Sensors.h"
#include "Utils.h"
#include <stdint.h>
#include <string.h>
#include"i2c0.h"
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

char rx_buffer[30];

uint16_t bufferIndex = 0;
char *command;
char *command1;
const char s[2] = "=";

char* test;
int number;

void SendName()
{
	SendAccelData("x");
	SendAccelData(",");
	SendAccelData("y");
	SendAccelData(",");
	SendAccelData("z");
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

void SendTest()
{
	uint32_t i;
	static int32_t mpuXVal=0;

	static int32_t mpuYVal=0;

	static int32_t mpuZVal=0;

	mpuXVal=ReadSensors(xAxis);


	mpuYVal=ReadSensors(yAxis);


	mpuZVal=ReadSensors(zAxis);


	char xVal[16];
	char yVal[16];
	char zVal[16];

	strcpy(xVal, IntToString(mpuXVal));
    strcpy(yVal, IntToString(mpuYVal));
    strcpy(zVal, IntToString(mpuZVal));

    SendUartTest(xVal,yVal,zVal);

	for(i=0;i<=100000;i++);
}


int main(void)
{
    WDT_A_holdTimer();
    InitSensors(mpu);
    InitUarts(uartModule0);
    InitUartReceive();

    while(1)
    {
    	if(rx_buffer!='\0')
    	{
    		if(rx_buffer[bufferIndex-1]==125)
    		{
    			UartCommands(rx_buffer);
    			memset(rx_buffer,'\0',sizeof(rx_buffer));
    	    	bufferIndex=0;
    		}
    	}
    }
}


void euscia0_isr(void)
{
	 if (UCA0IFG & UCRXIFG)
	    {
		 	 rx_buffer[bufferIndex] = UCA0RXBUF;
		 	 bufferIndex++;
	    }
}








