/******************************************************************************
 *
 * File Name: MC_1.c
 *
 * Description: Door Locker Security System MC 1
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/

#include "keypad.h"
#include "LCD.h"
#include "MC_1.h"
#include "timer_0.h"
#include "uart.h"
#include <avr/io.h> /* to enable the global interrupt*/
#include <util/delay.h>


/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/

uint8 g_inputPassword[PASSWORD_LENGTH];/* to store the password inputed from the user */

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

	/* Initialize LCD */
	LCD_init();

	/* Set the Password for the first time */
	MC1_newPassword();

	while(1)
	{
		/* Display the main options */
		MC1_mainMenueOptions();

		/* Store which key has been pressed */
		key_option = KEYPAD_getPressedKey();

		switch (key_option)
		{
		case OPEN_DOOR:

			/* Ask the user to input a password */
			MC1_requestPassword();
			/* Ask Mc2 to check the Password */
			MC1_sendCommand(SEND_CHECK_PASSWORD);
			/* Send the inputed password to MC2 */
			MC1_sendPassword(g_inputPassword);
			/* Inform MC2 what the user has chosen */
			MC1_sendCommand(OPEN_DOOR);

			/* Receive the state command from MC2 */
			g_matchStatus = MC1_receiveCommand();

			/* In case the two passwords matches */
			if(g_matchStatus ==  OPENING_DOOR)
			{
				MC1_openingDoor();
			}
			/* In case the two passwords did not match */
			else if(g_matchStatus == WRONG_PASSWORD)
			{
				MC1_wrongPassword();
			}
			break;

		case CHANGE_PASSWORD:

			/* Ask the user to input a password */
			MC1_requestPassword();
			/* Ask MC2 to check the Password */
			MC1_sendCommand(SEND_CHECK_PASSWORD);
			/* Send the inputed password to the MC2 */
			MC1_getPassword(g_inputPassword);
			/* Inform MC2 what the user has chosen */
			MC1_sendCommand(CHANGE_PASSWORD);

			/* Receive the order command from MC2 */
			g_matchStatus = MC1_receiveCommand();

			/* In case the two passwords matches */
			if(g_matchStatus ==  CHANGING_PASSWORD)
			{
				MC1_newPassword();
			}
			/* In case the two passwords did not match */
			else if(g_matchStatus == WRONG_PASSWORD)
			{
				MC1_wrongPassword();
			}
			break;

		}
	}
}

/*
 * Description:
 * Function to set the call back function for the Interrupt to be executed
 */
void MC1_TimerCallBack(void)
{
	g_tick++; /* Increment the ticks */
}

/*
 * Description:
 * Function to initialize Timer
 */
void MC1_startTimer(void)
{
	/* Setup Timer 0 Configuration */
	Timer0_ConfigType TIMER_Config = {Timer0_CTC, Timer0_1024_Prescaler, 0, 7813 };

	/* Initialize the Timer */
	Timer0_init(&TIMER_Config);

	/* Set Call Back function for the timer */
	Timer0_SetCallBack(MC1_TimerCallBack);
}

/*
 * Description:
 * Function to send specific commands to the MC2 through UART
 */
void MC1_sendCommand(uint8 g_command)
{
	/* Inform MC2 that you are to send */
	UART_sendByte(READY_TO_SEND);

	/* Wait until MC2 are ready to receive */
	while(UART_recieveByte() != READY_TO_RECEIVE);

	/* Send the required command to MC2 */
	UART_sendByte(g_command);

	/* Wait until the MC2 receive the command */
	while(UART_recieveByte() != RECEIVE_DONE);
}

/*
 * Description:
 * Function to receive specific command from the MC2 through UART
 * and Direct the MC2 to do predefined set of operations
 */
uint8 MC1_receiveCommand(void)
{
	/* Wait until the MC2 is ready to send */
	while(UART_recieveByte() != READY_TO_SEND);

	/* Inform the MC2 that you are ready to receive */
	UART_sendByte(READY_TO_RECEIVE);

	/* Receive the command from the MC2 */
	g_command = UART_recieveByte();

	/* Inform the MC2 that the receive has been done successfully */
	UART_sendByte(RECEIVE_DONE);

	return g_command;
}

/*
 * Description:
 * Function to set a new Password
 */
void MC1_newPassword(void)
{
	/* Loop until the MC1 get the same password */
	while(g_matchStatus == PASS_MIS_MATCHED)
	{

		LCD_clearScreen();
		LCD_displayString("Enter Password"); /* request the password for the first time */
		LCD_moveCursor(1,0); /* Move Cursor to the second line */
		MC1_getPassword(g_inputPassword); /* Get the password from the user */

		MC1_sendCommand(SEND_FIRST_PASSWORD); /* Inform the MC2 that you will send the first password */
		MC1_sendPassword(g_inputPassword); /* Send the password to MC2 */


		LCD_clearScreen();
		LCD_displayString("ReEnter The Password"); /* request the password for the second time */
		LCD_moveCursor(1,0); /* Move Cursor to the second line */
		MC1_getPassword(g_inputPassword); /* Get the password from the user */

		MC1_sendCommand(SEND_SECOND_PASSWORD); /* Inform the MC2 that you will send the second password */
		MC1_sendPassword(g_inputPassword); /* Send the password to MC2 */

		/* Wait until the MC2 is able to send the confirmation of the second password */
		g_matchStatus = MC1_receiveCommand();

		/* In case the Passwords didn't match */
		if (g_matchStatus == PASS_MIS_MATCHED)
		{
			LCD_clearScreen(); /* Clear Screen */
			LCD_displayString("Wrong Password"); /* Display an Error Message */

		}
	}
}

/*
 * Description:
 * Function that takes Password characters form array
 * and Send that password to MC2 through UART
 */
void MC1_sendPassword(uint8 a_inputPassword[])
{
	uint8 counter;

	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		UART_sendByte(a_inputPassword[counter]); /* Send Password element by element to the MC2 */
		_delay_ms(SEND_RECEIVE_TIME);      /* Delay for the time gap for sending receiving time between the MCUs */
	}
}

/*
 * Description:
 * Function that takes Password from Keypad
 * and Store it in array and Display asterisk on the screen
 */
void MC1_getPassword(uint8 a_inputPassword[])
{
	LCD_moveCursor(2, 0);

	uint8 counter = 0; /* Variable to be used as a counter */
	uint8 password_key = 0; /* Variable to store the pressed key */

	/* Stop getting number after you get 5 characters */
	while( counter != PASSWORD_LENGTH )
	{
		password_key = KEYPAD_getPressedKey(); /* Get the get the key pressed and store it in the password array */

		if ( (password_key >= 0) && (password_key <= 9) )
		{
			LCD_displayCharacter('*'); /* Display asterisk to hide the password */
			a_inputPassword[counter] = password_key;
			counter++;
		}
		_delay_ms(KEYPAD_CLICK_TIME); /* Delay time for keypad press */
	}
}

/*
 * Description:
 * Function that displays the main options for the program
 */
void MC1_mainMenueOptions(void)
{
	LCD_clearScreen();
	LCD_displayString("(+): Open Door");
	LCD_moveCursor(1,0); /* Move to the next line */
	LCD_displayString("(-): Change Password");
}

/*
 * Description:
 * Request a password
 */
void MC1_requestPassword(void)
{
	LCD_clearScreen();
	LCD_displayString("Enter The Password :");
	MC1_getPassword(g_inputPassword);
}

/*
 * Description:
 * Function that explain door phase on the screen
 */
void MC1_openingDoor(void)
{
	MC1_startTimer(); /* Start the timer to measure time period */

	/* Open the door for ( 15 sec ) */
	LCD_clearScreen();
	LCD_displayString("Door is Opening");
	while(g_tick < OPEN_DOOR_TIME); /* Count up to 15 */
	g_tick = 0; /* Reset counter to reuse it */

	/* Close the door for ( 15 sec ) */
	LCD_clearScreen();
	LCD_displayString("Door is Closing");
	while(g_tick < CLOSE_DOOR_TIME); /* Count up to 15 */
	g_tick = 0; /* Reset counter to reuse it */

	Timer0_Deinit();
	LCD_clearScreen();
}

/*
 * Description:
 * Function that take care of wrong password scenarios
 */
void MC1_wrongPassword(void)
{
	g_passwordMistakes++; /* Increment the wrong counter */

	LCD_clearScreen();
	LCD_displayString(" Wrong Password ");

	/* If the user entered the password 3 times wrong */
	if(g_passwordMistakes == MAX_NUM_OF_MISTAKES)
	{
		MC1_startTimer(); /* Start the timer to measure time period */

		/* Reset the counters */
		g_passwordMistakes = 0;
		g_tick = 0;

		Timer0_Deinit(); /* Stop the timer */
	}
	LCD_clearScreen();
}
