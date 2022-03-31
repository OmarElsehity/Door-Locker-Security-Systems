/******************************************************************************
 *
 * Module: Timer 0
 *
 * File Name: timer_0.h
 *
 * Description: Header file for Timer 0 driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/

#ifndef TIMER_0_H_
#define TIMER_0_H_

#include "std_types.h"  /*to use uint8 data type*/

/*******************************************************************************
 *                            Configurations                                   *
 *******************************************************************************/
typedef enum{
	Timer0_normal, Timer0_CTC
}Timer0_mode;

typedef enum{
	Timer0_NoClock, Timer0_NoPrescaler, Timer0_8_Prescaler, Timer0_64_Prescaler, Timer0_256_Prescaler,
	Timer0_1024_Prescaler, Timer0_External_clock_failling, Timer0_External_clock_rising
}Timer0_clock;

typedef struct{
	Timer0_mode mode;
	Timer0_clock clock;
	uint8 initial_value;
	uint8 compare_value;
}Timer0_ConfigType;

/*******************************************************************************
 *                           Functions Prototypes                              *
 *******************************************************************************/

/*
 * Initialization of the timer 0
 */
void Timer0_init(const Timer0_ConfigType* ConfigType);

/*
 * De-initialization of timer 0
 */
void Timer0_Deinit();

/*
 * Set callback function used by timer 0
 */
void Timer0_SetCallBack( void(*ptr2func)(void) );



#endif /* TIMER_0_H_ */
