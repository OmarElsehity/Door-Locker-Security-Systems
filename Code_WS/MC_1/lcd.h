/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* LCD HW Ports and Pins Ids */
#define LCD_4BITS_MODE FALSE

#define LCD_RS_PORT PORTD_ID
#define LCD_RS_PIN PIN4_ID
#define LCD_RW_PORT PORTD_ID
#define LCD_RW_PIN PIN5_ID
#define LCD_E_PORT PORTD_ID
#define LCD_E_PIN PIN6_ID

#define LCD_DATA_PORT PORTC_ID
#if(LCD_4BITS_MODE == TRUE)
#define FIRST_4_BITS
#ifdef FIRST_4_BITS
#define LCD_DATA_PIN PIN0_ID
#else
#define LCD_DATA_PIN PIN4_ID
#endif
#endif
/* LCD Commands */
#define LCD_GO_TO_HOME                  0x02
#define LCD_TWO_LINES_EIGHT_BITS_MODE   0x38
#define CLEAR_DISPLAY 					0x01
#define DISPLAY_ON_CURSOR_ON			0x0E
#define DISPLAY_ON_CURSOR_OFF 			0x0C
#define LCD_TWO_LINES_FOUR_BITS_MODE    0x28
#define LCD_SET_CURSOR_LOCATION         0x80

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void);
/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command);

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 character);
/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(uint8 *str);

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row,uint8 col);

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,uint8 *str);
/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void);

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data);

#endif /* LCD_H_ */
