/******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                            Configurations                                   *
 *******************************************************************************/
typedef enum{
	TWI_Prescaler_1,TWI_Prescaler_4,TWI_Prescaler_16,TWI_Prescaler_64
}TWI_Prescaler;

typedef struct{
	uint8 BitRateRegister_TWBR;

	TWI_Prescaler prescaler;

	uint8 My_Address;

}TWI_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * initialization function of TWI using Configuration structure
 */
void TWI_init(const TWI_ConfigType* config_ptr );

/*
 * function to Send the Start Bit
 */
void TWI_start(void);

/*
 * function to Send the stop Bit
 */
void TWI_stop(void);

/*
 * function to write one byte (8 bits) of data
 */
void TWI_writeByte(uint8 data);

/*
 * function to read one byte of data with Acknowledge
 */
uint8 TWI_readByteWithACK(void);

/*
 * function to read one byte of data with NACK
 */
uint8 TWI_readByteWithNACK(void);

/*
 * function to get status of TWI module
 */
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
