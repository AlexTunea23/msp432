
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
#include "Accelerometer.h"

//***** Definitions *****

#define TIMER_PERIOD   25000
#define CPU_FREQ                    (48000000)


//***** Function Prototypes *****

void setSystemClock(uint32_t CPU_Frequency);
void startCrystalOscillator(void);



//volatile uint8_t counter;

//***** Global Data *****

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,           // SMCLK = 48 MHZ
		TIMER_PERIOD,                           // 2microsec
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,// Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};













char rxxx_buffer[30];
char *command_buffer[30];

uint16_t bufferIndex = 0;
char *command;
char *command1;
const char s[2] = "=";

char* test;
int number;


#define BUFFER_SIZE  90
struct Buffer {
   int data[BUFFER_SIZE];
    int newest_index;
    int oldest_index;
};

enum BufferStatus {BUFFER_OK, BUFFER_EMPTY, BUFFER_FULL};

volatile struct Buffer rx_buffer = {{}, 0, 0};
enum BufferStatus status;




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


void startCrystalOscillator(void)
{
    /* Configuring pins for peripheral/crystal HFXT*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN3 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring pins for peripheral/crystal LFXT*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
}

void setSystemClock(uint32_t CPU_Frequency)
{
    MAP_CS_setExternalClockSourceFrequency(32768, CPU_Frequency);

    /* Before we start we have to change VCORE to 1 to support the 48MHz frequency */
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    PCM_setPowerState(PCM_AM_LDO_VCORE1);

    MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

    /* Starting HFXT and LFXT in non-bypass mode without a timeout. */
    MAP_CS_startHFXT(false);
    MAP_CS_startLFXT(false);

    /* Initializing the clock source as follows:
     *      MCLK = HFXT = 48MHz
     *      ACLK = LFXT = 32KHz
     *      HSMCLK = HFXT/4 = 12MHz
     *      SMCLK = HFXT/8 = 6MHz
     *      BCLK  = REFO = 32kHz
     */
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_8);
    MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_4);
    MAP_CS_initClockSignal(CS_BCLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}




void initTimers()
{
	 startCrystalOscillator();
	    setSystemClock(CPU_FREQ);

	    int valueX;
	    //

	    /* Configuring P1.0 and P1.5 as output */
	        MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
	        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
	        MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

	    /* Configuring Timer_A1 and TimerA_0 for Up Mode */
	        MAP_Timer_A_configureUpMode(TIMER_A1_MODULE, &upConfig);
	        MAP_Timer_A_configureUpMode(TIMER_A0_MODULE, &upConfig);


	        /* Enabling interrupts and starting the timer */
	        MAP_Interrupt_enableSleepOnIsrExit();
	        MAP_Interrupt_enableInterrupt(INT_TA1_0);
	        MAP_Timer_A_startCounter(TIMER_A1_MODULE, TIMER_A_UP_MODE);
	        MAP_Interrupt_enableInterrupt(INT_TA0_0);
	        MAP_Timer_A_startCounter(TIMER_A0_MODULE, TIMER_A_UP_MODE);



	        /* Enabling MASTER interrupts */

	        MAP_Interrupt_enableMaster();

}




void send()
{
	int received_byte;
				    char xVal[16];
				    char yVal[16];
				    char zVal[16];


			        status = bufferRead(&rx_buffer, &received_byte);
			    	strcpy(xVal, IntToString(received_byte));
			    	SendAccelData(xVal);
			        SendAccelData(",");

			        status = bufferRead(&rx_buffer, &received_byte);
			        strcpy(yVal, IntToString(received_byte));
			        SendAccelData(yVal);
			        SendAccelData(",");

			        status = bufferRead(&rx_buffer, &received_byte);
			        strcpy(zVal, IntToString(received_byte));
			        SendAccelData(zVal);
			        SendAccelData(",");
			        SendCharacterData(10);
			        SendCharacterData(13);
}


void add()
{
					int valueX;
					int valueY;
			    	int valueZ;
					valueX=ReadSensors(xAxis);
			    	valueY=ReadSensors(yAxis);
			    	valueZ=ReadSensors(zAxis);
			    	bufferWrite(&rx_buffer, valueX);
			    	bufferWrite(&rx_buffer, valueY);
			    	bufferWrite(&rx_buffer, valueZ);
}

int main(void)
{
    WDT_A_holdTimer();
    InitSensors(mpu);
    InitUarts(uartModule0);
  //  InitUartReceive();
    //initTimers();
    int valueX;
    char xVal[16];
    char yVal[16];
    char zVal[16];
    char tempp[16];
    int i=0;

    int valueY, valueZ, temp;



    while(1)
    {
    	//add();
    	//send();

    	valueX=ReadSensors(xAxisGyro);
    	valueY=ReadSensors(yAxisGyro);
    	valueZ=ReadSensors(zAxisGyro);

    	temp=ReadSensors(Tempp);



    	strcpy(xVal, IntToString(valueX));
    	SendAccelData(xVal);
    	SendAccelData(",");

    	strcpy(yVal, IntToString(valueY));
    	SendAccelData(yVal);
    	SendAccelData(",");

    	strcpy(zVal, IntToString(valueZ));
    	SendAccelData(zVal);
    	SendAccelData(",");
    	strcpy(tempp, IntToString(temp));
    	SendAccelData(tempp);

    	SendCharacterData(10);
    	SendCharacterData(13);
    	for(i=0;i<10000;i++);








    	/* if (status == BUFFER_OK)
    	        {
    	        		strcpy(xVal, IntToString(received_byte));
    	        		SendAccelData(xVal);
    	        		SendCharacterData(10);
    	        		SendCharacterData(13);
    	        		for(i=0;i<=100000;i++);
    	        }*/


    	/*if(rx_buffer!='\0' && rx_buffer[bufferIndex-1]==125)
    	{

    			UartCommands(rx_buffer);
    			memset(rx_buffer,'\0',sizeof(rx_buffer));
    			bufferIndex=0;
    	}*/

    	//MAP_PCM_gotoLPM0();

    }
}


void euscia0_isr(void)
{
	 if (UCA0IFG & UCRXIFG)
	    {
		 	 rxxx_buffer[bufferIndex] = UCA0RXBUF;
		 	 bufferIndex++;
	    }
}

void timer_a_0_isr(void)
{

	//GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
	add();
	//GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);

					//add();
			    	MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A1_MODULE,
		            TIMER_A_CAPTURECOMPARE_REGISTER_0);
}

void timer_a_1_isr(void)
{
		GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
		send();
		GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);

		        MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_MODULE,
		       TIMER_A_CAPTURECOMPARE_REGISTER_0);
}







