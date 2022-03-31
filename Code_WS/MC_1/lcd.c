/******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.c
 *
 * Description: Source file for the LCD driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/
#include <util/delay.h>/* For the delay functions */
#include "common_macros.h"/* To use the macros like SET_BIT */
#include "lcd.h"
#include "gpio.h"
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_init(void){
	/* Configure the direction for RS, RW and E pins as output pins */
	GPIO_setupPinDirection(LCD_RS_PORT,LCD_RS_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_RW_PORT,LCD_RW_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_E_PORT,LCD_E_PIN,PIN_OUTPUT);

#if(LCD_4BITS_MODE==TRUE)
	/* Configure 4 pins in the data port as output pins */
	GPIO_setupPinDirection(LCD_DATA_PORT,LCD_DATA_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT,LCD_DATA_PIN+1,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT,LCD_DATA_PIN+2,PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_DATA_PORT,LCD_DATA_PIN+3,PIN_OUTPUT);
	LCD_sendCommand(LCD_GO_TO_HOME);
	LCD_sendCommand(LCD_TWO_LINES_FOUR_BITS_MODE);/* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
#else
	/* Configure the data port as output port */
	GPIO_setupPortDirection(LCD_DATA_PORT,PORT_OUTPUT);
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);/* use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */

#endif


	LCD_sendCommand(DISPLAY_ON_CURSOR_OFF);
	LCD_sendCommand(CLEAR_DISPLAY);
}
/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(uint8 command){

	GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,LOGIC_LOW);
	GPIO_writePin(LCD_RW_PORT,LCD_RW_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_HIGH);
	_delay_ms(1);
#if(LCD_4BITS_MODE==TRUE)
	lcd_data=GPIO_readPort(LCD_DATA_PORT);
#ifdef FIRST_4_BITS
	lcd_data=(lcd_data & 0xF0) | ((command & 0xF0) >>4);
#else
	lcd_data = (lcd_data & 0x0F) | (command & 0xF0);
#endif
	GPIO_writePort(LCD_DATA_PORT,lcd_data);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_HIGH);
	_delay_ms(1);
	lcd_data=GPIO_readPort(LCD_DATA_PORT);
#ifdef FIRST_4_BITS
	lcd_data=(lcd_data & 0xF0) | (command & 0x0F);
#else
	lcd_data = (lcd_data & 0x0F) | ((command & 0x0F) << 4);
#endif
	GPIO_writePort(LCD_DATA_PORT,lcd_data);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);
	_delay_ms(1);
#else
	GPIO_writePort(LCD_DATA_PORT,command);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);
	_delay_ms(1);
#endif
}
/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(uint8 character){

	GPIO_writePin(LCD_RS_PORT,LCD_RS_PIN,LOGIC_HIGH);
	GPIO_writePin(LCD_RW_PORT,LCD_RW_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_HIGH);
	_delay_ms(1);
#if(LCD_4BITS_MODE==TRUE)
	lcd_data=GPIO_readPort(LCD_DATA_PORT);
#ifdef FIRST_4_BITS
	lcd_data=(lcd_data & 0xF0) | ((character & 0xF0) >>4);
#else
	lcd_data = (lcd_data & 0x0F) | (character & 0xF0);
#endif
	GPIO_writePort(LCD_DATA_PORT,lcd_data);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_HIGH);
	_delay_ms(1);
	lcd_data=GPIO_readPort(LCD_DATA_PORT);
#ifdef FIRST_4_BITS
	lcd_data=(lcd_data & 0xF0) | (character & 0x0F);
#else
	lcd_data=(lcd_data & 0x0F) | ((character & 0x0F)<<4);
#endif
	GPIO_writePort(LCD_DATA_PORT,lcd_data);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);
	_delay_ms(1);
#else
	GPIO_writePort(LCD_DATA_PORT,character);
	_delay_ms(1);
	GPIO_writePin(LCD_E_PORT,LCD_E_PIN,LOGIC_LOW);
	_delay_ms(1);
#endif

}
/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(uint8 *str){
	while (*str != '\0'){
		LCD_displayCharacter(*str);
		str++;
	}
}
/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void){
	LCD_sendCommand(CLEAR_DISPLAY);
}
/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(uint8 row ,uint8 col){
	uint8 lcd_address;
	switch(row){
	case 1:
		lcd_address=col;
		break;
	case 2:
		lcd_address=col+0X40;
		break;
	case 3:
		lcd_address=col+0x10;
		break;
	case 4:
		lcd_address=col+0x50;
	}
	LCD_sendCommand(lcd_address |LCD_SET_CURSOR_LOCATION);
}
/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(uint8 row,uint8 col,uint8 *str){
	LCD_moveCursor(row ,col);
	LCD_displayString(str);
}
/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data){
	char num[16];
	itoa(data,num,10);
	LCD_displayString(num);
}
