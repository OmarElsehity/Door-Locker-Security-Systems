/******************************************************************************
 *
 * File Name: MC_1.h
 *
 * Description: Door Locker Security System MC 1
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/
#ifndef MC_1_H_
#define MC_1_H_
#define F_CPU 8000000UL //8MHz Clock frequency
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Definitions for Hand Shaking Protocol */
#define OPEN_DOOR             			'+'
#define CHANGE_PASSWORD       			'-'
#define OPENING_DOOR          			0xF0
#define WRONG_PASSWORD        			0xF1
#define CHANGING_PASSWORD     			0xF2
#define READY_TO_SEND         			0xF3
#define READY_TO_RECEIVE      			0xF4
#define RECEIVE_DONE          			0xF5
#define SEND_FIRST_PASSWORD   			0xF6
#define SEND_SECOND_PASSWORD 			0xF7
#define SEND_CHECK_PASSWORD   			0xF8

/* Definitions for Password */
#define PASSWORD_LENGTH         		5
#define MAX_NUM_OF_MISTAKES     		3
#define PASS_MIS_MATCHED              	0
#define PASS_MATCHED				  	1

/* Definitions for Time Periods */
#define SEND_RECEIVE_TIME      			10
#define KEYPAD_CLICK_TIME         		500
#define OPEN_DOOR_TIME      			15
#define CLOSE_DOOR_TIME      			15
#define WARNING_TIME           			60

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Function to set the call back function for the Interrupt to be executed
 */
void MC1_TimerCallBack(void);

/*
 * Description:
 * Function to initialize Timer
 */
void MC1_startTimer(void);

/*
 * Description:
 * Function to send specific commands to the MC2 through UART
 */
void MC1_sendCommand(uint8 g_command);

/*
 * Description:
 * Function to receive specific command from the MC2 through UART
 * and Direct the MC2 to do predefined set of operations
 */
uint8 MC1_receiveCommand(void);

/*
 * Description:
 * Function to set a new Password
 */
void MC1_newPassword(void);

/*
 * Description:
 * Function that takes Password characters form array
 * and Send that password to MC2 through UART
 */
void MC1_sendPassword(uint8 a_inputPassword[]);

/*
 * Description:
 * Function that takes Password from Keypad
 * and Store it in array and Display asterisk on the screen
 */
void MC1_getPassword(uint8 a_inputPassword[]);

/*
 * Description:
 * Function that displays the main options for the program
 */
void MC1_mainMenueOptions(void);

/*
 * Description:
 * Request a password
 */
void MC1_requestPassword(void);

/*
 * Description:
 * Function that explain door phase on the screen
 */
void MC1_openingDoor(void);

/*
 * Description:
 * Function that take care of wrong password scenarios
 */
void MC1_wrongPassword(void);

#endif /* MC_1_H_ */
