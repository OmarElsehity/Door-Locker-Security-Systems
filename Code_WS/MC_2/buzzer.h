 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: header file for the buzzer driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/

#ifndef HAL_BUZZER_BUZZER_H_
#define HAL_BUZZER_BUZZER_H_


#include "gpio.h"

/*******************************************************************************
 *                               Definitions                                   *
 *******************************************************************************/

#define Buzzer_Port PORTA_ID
#define Buzzer_PIN PIN0_ID

/*******************************************************************************
 *                          Functions Prototypes                               *
 *******************************************************************************/

/*
 * function to initialize buzzer
 */
void BUZZER_init();

/*
 * function to turn on buzzer
 */
void BUZZER_ON();

/*
 * function to turn off buzzer
 */
void BUZZER_OFF();


#endif /* HAL_BUZZER_BUZZER_H_ */
