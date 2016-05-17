
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
#include "msp432.h"
#include "Timers.h"





char receive_buffer[30];
char *command_buffer[30];

uint16_t bufferIndex = 0;
char *command;
char *command1;
const char s[2] = "=";
uint8_t startFlag=0;


uint8_t flagTreshold=0;
uint32_t setTreshold=0;




int main(void)
{
    WDT_A_holdTimer();
    InitSensors(mpu);
    InitUarts(uartModule0);
    InitUartReceive();
    InitTimers();


    while(1)
    {
    	if(receive_buffer!='\0' && receive_buffer[bufferIndex-1]==125)
    	{

    			UartCommands(receive_buffer);
    			memset(receive_buffer,'\0',sizeof(receive_buffer));
    			bufferIndex=0;
    	}
    	MAP_PCM_gotoLPM0();
    }
}


void euscia0_isr(void)
{
	 if (UCA0IFG & UCRXIFG)
	    {
		 	 receive_buffer[bufferIndex] = UCA0RXBUF;
		 	 bufferIndex++;
	    }
}

void timer_a_1_isr(void)
{
	if(flagTreshold==0)
	{
	setTreshold=GetTreshold();
	flagTreshold=WaitUntilStart(setTreshold);
	StartTimerA0_0();
	}
	if(flagTreshold==1)
	{
	Add();
	}
	MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_MODULE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
}


void timer_a_0_isr(void)
{
	MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
	Send();
	MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_MODULE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
}







