/*
 * Timers.c
 *
 *  Created on: May 9, 2016
 *      Author: PetruAlexandru
 */

#include"Timers.h"
#include<driverlib.h>


#define TIMER_PERIOD   				20000
#define CPU_FREQ                    (48000000)


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


void startCrystalOscillator(void)
{
    /* Configuring pins for peripheral/crystal HFXT*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN3 | GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring pins for peripheral/crystal LFXT*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,
            GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
}


void initTimers()
{
	startCrystalOscillator();
	setSystemClock(CPU_FREQ);
	/* Configuring P1.0 and P1.5 as output */
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
	MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
	MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

		    /* Configuring Timer_A1 and TimerA_0 for Up Mode */

	/* Enabling interrupts and starting the timer */
	MAP_Interrupt_enableSleepOnIsrExit();
	MAP_Interrupt_enableInterrupt(INT_TA1_0);
	MAP_Timer_A_startCounter(TIMER_A1_MODULE, TIMER_A_UP_MODE);
	MAP_Interrupt_enableInterrupt(INT_TA0_0);
	MAP_Timer_A_startCounter(TIMER_A0_MODULE, TIMER_A_UP_MODE);
		        /* Enabling MASTER interrupts */
	MAP_Interrupt_enableMaster();
}


void SetTimersPeriod(uint32_t periodConfig)
{
	const Timer_A_UpModeConfig upConfig =
	{
	        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK Clock Source
	        TIMER_A_CLOCKSOURCE_DIVIDER_1,           // SMCLK = 48 MHZ
			periodConfig,
	        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
	        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,// Enable CCR0 interrupt
	        TIMER_A_DO_CLEAR                        // Clear value
	};

	MAP_Timer_A_configureUpMode(TIMER_A1_MODULE, &upConfig);
	MAP_Timer_A_configureUpMode(TIMER_A0_MODULE, &upConfig);
}


