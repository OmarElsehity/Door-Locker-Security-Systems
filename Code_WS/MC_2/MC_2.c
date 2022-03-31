/******************************************************************************
 *
 * File Name: MC_2.c
 *
 * Description: Door Locker Security System MC 2
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "MC_2.h"
#include "external_eeprom.h"
#include "motor.h"
#include "buzzer.h"
#include "uart.h"
#include "twi.h"
#include "timer_0.h"

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/

uint8 g_storedPassword[PASSWORD_LENGTH]; /* to store the password saved in the external EEPROM */

uint8 g_receivedPassword[PASSWORD_LENGTH];/* to store the first password inputed from the user */

uint8 g_confirmPassword[PASSWORD_LENGTH];/* to store the second password inputed from the user */

uint8 g_matchStatus = PASS_MIS_MATCHED;/* to store the status of the Password after comparing */

uint8 g_tick = 0;/* to keep track of the seconds counted by the timer */

uint8 g_passwordMistakes = 0;/* to keep track of how many wrong password trials */

uint8 g_command;/* to keep track of the command sent from the MC2 through UART */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

int main(void)
{
	/* Variable to store the pressed key */
	uint8 key_option;

	SREG  |= ( 1 << 7 ); /* Enable Global Interrupts */

	/* Initialize the UART with Configuration */
	UART_ConfigType UART_Config = {9600, EIGHT_BITS, ONE_STOP_BIT, DISABLED};
	UART_init(&UART_Config);

	/* Initialize I2C with Configuration */
	TWI_ConfigType TWI_Config = {400000, TWI_Prescaler_1, TWI_ADDRESS};
	TWI_init(&TWI_Config);

	/* Initialize DC Motor */
	DcMotor_Init();

	/* Initialize Buzzer */
	BUZZER_init();

	/* Save the Password for the first time */
	MC2_newPassword();

	while(1)
	{
		/* Wait until the MC1 send the inputed password */
		while(MC2_receiveCommand() != SEND_CHECK_PASSWORD);
		/* Receive the inputed password and store it */
		MC2_receivePassword(g_receivedPassword);
		/* Receive the command from the MC1 */
		key_option = MC2_receiveCommand();

		switch (key_option)
		{
		case OPEN_DOOR:

			/* Read Password from the EEPROM */
			MC2_readPassword(g_storedPassword);
			/* Compare it with the given from user */
			g_matchStatus = MC2_comparePasswords(g_receivedPassword, g_storedPassword);

			/* In case the two passwords matches */
			if(g_matchStatus == PASS_MATCHED)
			{
				/* Send Opening Door command to MC1 */
				MC2_sendCommand(OPENING_DOOR);
				MC2_openingDoor();
			}
			/* In case the two passwords did not match */
			else if(g_matchStatus == PASS_MIS_MATCHED)
			{
				/* Send Wrong Password command to MC1 */
				MC2_sendCommand(WRONG_PASSWORD);
				MC2_wrongPassword();
			}
			break;

		case CHANGE_PASSWORD:

			/* Read Password from the EEPROM */
			MC2_readPassword(g_storedPassword);
			/* Compare it with the inputed user */
			g_matchStatus = MC2_comparePasswords(g_receivedPassword, g_storedPassword);

			/* In case the two passwords matches */
			if(g_matchStatus == PASS_MATCHED)
			{
				/* Send Changing Password command to MC1 */
				MC2_sendCommand(CHANGING_PASSWORD);
				MC2_newPassword();
			}
			/* In case the two passwords did not match */
			else if(g_matchStatus == PASS_MIS_MATCHED)
			{
				/* Send Wrong Password command to MC1 */
				MC2_sendCommand(WRONG_PASSWORD);
				MC2_wrongPassword();
			}
			break;
		}
	}
}

/*
 * Description:
 * Function to set the call back function for the Interrupt to be executed
 */
void MC2_TimerCallBackProcessing(void)
{
	g_tick++; /* Increment the counter */
}

/*
 * Description:
 * Function to initialize Timer
 */
void MC2_startTimer(void)
{
	/* Setup Timer Configuration */
	Timer0_ConfigType TIMER_Config = {Timer0_CTC, Timer0_1024_Prescaler, 0, 7813 };

	/* Initialize the Timer */
	Timer0_init(&TIMER_Config);

	/* Set Call Back function for the timer */
	Timer0_SetCallBack(MC2_TimerCallBackProcessing);
}

/*
 * Description:
 * Function to set a new Password
 */
void MC2_newPassword(void)
{
	g_matchStatus = PASS_MIS_MATCHED;

	/* Loop until the MC1 get the same password */
	while(g_matchStatus == PASS_MIS_MATCHED)
	{
		/* Wait until the MC1 send command for first password */
		while (MC2_receiveCommand() != SEND_FIRST_PASSWORD );
		/* Receive the first password from MC1 */
		MC2_receivePassword(g_receivedPassword);

		/* Wait until the MC1 send command for second password */
		while (MC2_receiveCommand() != SEND_SECOND_PASSWORD );
		/* Receive the first password from MC1 */
		MC2_receivePassword(g_confirmPassword);

		/* Compare the Two received passwords */
		g_matchStatus = MC2_comparePasswords(g_receivedPassword, g_confirmPassword);

		/* In case the Two Passwords didn't match */
		if( g_matchStatus == PASS_MIS_MATCHED )
		{
			/* Send command informing that the passwords is wrong */
			MC2_sendCommand(PASS_MIS_MATCHED);
		}
		/* In case the Passwords matches */
		else if( g_matchStatus == PASS_MATCHED )
		{
			MC2_savePassword(g_receivedPassword);
			/* Send command informing that the passwords matched */
			MC2_sendCommand(PASS_MATCHED);
		}
	}
}

/*
 * Description :
 * Takes the Password which received from UART and Store it in an array
 */
void MC2_receivePassword(uint8 a_Password[])
{
	uint8 counter;

	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		a_Password[counter] = UART_recieveByte(); /* Receive Password from MC1 */
		_delay_ms(SEND_RECEIVE_TIME); /* Delay for the time gap for sending receiving time between the MCUs */
	}
}

/*
 * Description :
 * Function to compare two passwords received from MC1
 * and return the match status between them
 */
uint8 MC2_comparePasswords(uint8 a_password1[], uint8 a_password2[])
{
	uint8 counter;

	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		if (a_password1[counter] != a_password2[counter])
		{
			return PASS_MIS_MATCHED; /* Return passwords has not matched of one element mis-matched */
		}
	}

	return PASS_MATCHED;
}

/*
 * Description :
 * Function that save the matched password in external EEPROM
 */
void MC2_savePassword(uint8 a_receivedPassword[])
{
	uint8 counter;

	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		/* Save each element of the password in external EEPROM */
		EEPROM_writeByte( (0x0311+counter), a_receivedPassword[counter]);
		/* Delay for the time gap for storing data in EEPROM */
		_delay_ms(STORING_TIME);
	}
}

/*
 * Description :
 * Load the Password from EEPROM and Store it in an array
 */
void MC2_readPassword(uint8 a_storedPassword[])
{
	uint8 counter;

	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		/* Read each element of the password in external EEPROM */
		EEPROM_readByte( (0x0311+counter), &a_storedPassword[counter]);
		/* Delay for the time gap for storing data in EEPROM */
		_delay_ms(STORING_TIME);
	}
}

/*
 * Description:
 * Function that rotates the DC Motor
 */
void MC2_openingDoor(void)
{
	MC2_startTimer(); /* Start the Timer */

	DcMotor_Rotate(CW);
	while(g_tick != OPEN_DOOR_TIME); /* Count up to 15 */
	g_tick = 0; /* Reset counter to reuse it */

	DcMotor_Rotate(ACW);
	while(g_tick != CLOSE_DOOR_TIME); /* Count up to 15 */
	g_tick = 0; /* Reset counter to reuse it */

	DcMotor_Rotate(STOP); /* Stop the Motor */

	Timer0_Deinit();
}

/*
 * Description:
 * Function that take care of wrong password States
 */
void MC2_wrongPassword(void)
{
	g_passwordMistakes++; /* Increment the wrong counter */

	/* If the user entered the password 3 times wrong */
	if(g_passwordMistakes == MAX_NUM_OF_MISTAKES)
	{
		MC2_startTimer(); /* Start the timer to measure time period */

		BUZZER_ON(); /* Turn on the buzzer */
		while(g_tick != WARNING_TIME); /* Count up to 15 */

		/* Reset the counters */
		g_tick = 0;
		g_passwordMistakes = 0;

		Timer0_Deinit();
	}
	BUZZER_OFF(); /* Turn off the buzzer */
}

/*
 * Description:
 * Function to send specific commands to the MC1 through UART
 */
void MC2_sendCommand(uint8 g_command)
{
	/* Inform MC1 that you are to send */
	UART_sendByte(READY_TO_SEND);

	/* Wait until MC1 are ready to receive */
	while(UART_recieveByte() != READY_TO_RECEIVE);

	/* Send the required command to the MC1 */
	UART_sendByte(g_command);

	/* Wait until the MC1 receive the command */
	while(UART_recieveByte() != RECEIVE_DONE);
}

/*
 * Description:
 * Function to receive specific command from the MC1 through UART
 */
uint8 MC2_receiveCommand(void)
{
	/* Wait until the MC1 is ready to send */
	while(UART_recieveByte() != READY_TO_SEND);

	/* Inform the MC1 that you are ready to receive */
	UART_sendByte(READY_TO_RECEIVE);

	/* Receive the command from the MC1 */
	g_command = UART_recieveByte();

	/* Inform the MC1 that the receive has been done successfully */
	UART_sendByte(RECEIVE_DONE);

	return g_command;
}

