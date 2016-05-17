/*
 * Timers.c
 *
 *  Created on: May 9, 2016
 *      Author: PetruAlexandru
 */

#include"Timers.h"
#include<driverlib.h>
#include"stdint.h"


void InitTimers(void)
{/* Configuring pins for peripheral/crystal HFXT*/
	    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
	            GPIO_PIN3 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

	    /* Configuring pins for peripheral/crystal LFXT*/
	    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
	            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
	//setSystemClock(CPU_FREQ);
	 //Configuring P1.0 and P1.5 as output */
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
	/* Configuring Timer_A1 and TimerA_0 for Up Mode */
	/* Enabling interrupts and starting the timer */


	MAP_Interrupt_enableSleepOnIsrExit();

	MAP_Interrupt_enableMaster();
}

void StartTimerA1_0()
{
	MAP_Interrupt_enableInterrupt(INT_TA1_0);
	MAP_Timer_A_startCounter(TIMER_A1_MODULE, TIMER_A_UP_MODE);
}


void StartTimerA0_0()
{
	MAP_Interrupt_enableInterrupt(INT_TA0_0);
	MAP_Timer_A_startCounter(TIMER_A0_MODULE, TIMER_A_UP_MODE);
}


void StopTimerA1_0()
{
	MAP_Interrupt_disableInterrupt(INT_TA1_0);
}

void StopTimerA0_0()
{
	MAP_Interrupt_disableInterrupt(INT_TA0_0);
}



void SetTimerA1_0Period(uint32_t periodConfigA1)
{
	const Timer_A_UpModeConfig upConfig =
	{
	        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
	        TIMER_A_CLOCKSOURCE_DIVIDER_1,           // SMCLK = 48 MHZ
			periodConfigA1,
	        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
	        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,// Enable CCR0 interrupt
	        TIMER_A_DO_CLEAR                        // Clear value
	};

	MAP_Timer_A_configureUpMode(TIMER_A1_MODULE, &upConfig);
}


void SetTimerA0_0Period(uint32_t periodConfigA0)
{
	const Timer_A_UpModeConfig upConfig =
	{
	        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
	        TIMER_A_CLOCKSOURCE_DIVIDER_1,           // SMCLK = 48 MHZ
			periodConfigA0,
	        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
	        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,// Enable CCR0 interrupt
	        TIMER_A_DO_CLEAR                        // Clear value
	};

	MAP_Timer_A_configureUpMode(TIMER_A0_MODULE, &upConfig);
}


