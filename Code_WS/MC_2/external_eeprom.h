/******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * initialization of EEPROM
 */
void EEPROM_init(void);

/*
 * writing byte of data (8 bits) in specific memory location
 */
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);

/*
 * reading byte of data from specific memory location
 */
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */
