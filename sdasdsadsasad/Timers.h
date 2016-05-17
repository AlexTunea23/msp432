/*
 * Timers.h
 *
 *  Created on: May 9, 2016
 *      Author: PetruAlexandru
 */

#ifndef TIMERS_H_
#define TIMERS_H_
#include "stdint.h"


void setSystemClock(uint32_t CPU_Frequency);
void startCrystalOscillator(void);
void initTimers(void);
void SetTimersPeriod(uint32_t periodConfig);



#endif /* TIMERS_H_ */
