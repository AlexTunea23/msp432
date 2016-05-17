/*
 * Timers.h
 *
 *  Created on: May 9, 2016
 *      Author: PetruAlexandru
 */

#ifndef TIMERS_H_
#define TIMERS_H_
#include "stdint.h"



void startCrystalOscillator(void);
void InitTimers(void);
void SetTimersPeriod(uint32_t periodConfig);
void StartTimerA1_0();
void StartTimerA0_0();
void StopTimerA0_0();
void StopTimerA1_0();
void SetTimerA0_0Period(uint32_t periodConfigA0);
void SetTimerA1_0Period(uint32_t periodConfigA1);




#endif /* TIMERS_H_ */
