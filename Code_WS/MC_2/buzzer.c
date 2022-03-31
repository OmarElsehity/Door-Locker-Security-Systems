 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: source file for the buzzer driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/
#include "buzzer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * function to initialize buzzer
 */
void BUZZER_init(){
	GPIO_setupPinDirection(Buzzer_Port, Buzzer_PIN, PIN_OUTPUT);
	GPIO_writePin(Buzzer_Port, Buzzer_PIN, LOGIC_LOW);
}

/*
 * function to turn on buzzer
 */
void BUZZER_ON(){
	GPIO_writePin(Buzzer_Port, Buzzer_PIN, LOGIC_HIGH);
}

/*
 * function to turn off buzzer
 */
void BUZZER_OFF(){
	GPIO_writePin(Buzzer_Port, Buzzer_PIN, LOGIC_LOW);
}





