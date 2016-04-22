/*
 * Utils.c
 *
 *  Created on: Mar 8, 2016
 *      Author: PetruAlexandru
 */
#include "Utils.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Uart0.h"
#include "driverlib.h"
#include "stdlib.h"
#include "stdio.h"
#include "UartCommands.h"
#define INT_DIGITS 							19

#define MAX_POZ_VAL_2G						32768
#define MAX_VAL_2G							65536

#define MAX_POZ_VAL_G						32768
#define MAX_VAL_G							65536







int32_t HexaToDecG(int32_t value,int32_t lbs)
{
	const int32_t poz=MAX_POZ_VAL_G;
		const int32_t max=MAX_VAL_G;

		if(value>poz)
			{
				value=((value-max)*1000/lbs);
			}

		else
			{
				value=(value*1000/lbs);
			}
	return value;
}







char *IntToString(int32_t number)
{
  /* Room for INT_DIGITS digits, - and '\0' */
  static char buf[INT_DIGITS + 2];
  char *p = buf + INT_DIGITS + 1;
  if (number >= 0) {
    do {
      *--p = '0' + (number % 10);
      number /= 10;
    } while (number != 0);
    return p;
  }
  else {
    do {
      *--p = '0' - (number % 10);
      number /= 10;
    } while (number != 0);
    *--p = '-';
  }
  return p;
}










