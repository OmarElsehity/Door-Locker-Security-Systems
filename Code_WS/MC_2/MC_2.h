/******************************************************************************
 *
 * File Name: MC_2.h
 *
 * Description: Door Locker Security System MC 2
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/
#ifndef MC_2_H_
#define MC_2_H_
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
#define OPEN_DOOR_TIME      			15
#define CLOSE_DOOR_TIME      			15
#define WARNING_TIME           			60
#define STORING_TIME           			100

/* Definitions for TWI */
#define TWI_ADDRESS    0b0000001

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Function to set the call back function for the Interrupt to be executed
 */
void MC2_TimerCallBackProcessing(void);

/*
 * Description:
 * Function to initialize Timer
 */
void MC2_startTimer(void);

/*
 * Description:
 * Function to set a new Password
 */
void MC2_newPassword(void);

/*
 * Description :
 * Takes the Password which received from UART and Store it in an array
 */
void MC2_receivePassword(uint8 a_Password[]);

/*
 * Description :
 * Function to compare two passwords received from MC1
 * and return the match status between them
 */
uint8 MC2_comparePasswords(uint8 a_password1[], uint8 a_password2[]);

/*
 * Description :
 * Function that save the matched password in external EEPROM
 */
void MC2_savePassword(uint8 a_receivedPassword[]);

/*
 * Description :
 * Load the Password from EEPROM and Store it in an array
 */
void MC2_readPassword(uint8 a_storedPassword[]);

/*
 * Description:
 * Function that rotates the DC Motor
 */
void MC2_openingDoor(void);

/*
 * Description:
 * Function that take care of wrong password States
 */
void MC2_wrongPassword(void);

/*
 * Description:
 * Function to send specific commands to the MC1 through UART
 */
void MC2_sendCommand(uint8 g_command);

/*
 * Description:
 * Function to receive specific command from the MC1 through UART
 */
uint8 MC2_receiveCommand(void);

#endif /* MC_2_H_ */
