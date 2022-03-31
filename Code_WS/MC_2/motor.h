/******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: motor.h
 *
 * Description: Header file for the DC MOTOR driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_
#include "gpio.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MOTOR_PORT PORTD_ID
#define MOTOR_PIN1 PIN6_ID
#define MOTOR_PIN2 PIN7_ID

/*enum for motor state*/
typedef enum{
	STOP,CW,ACW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the DC Motor driver.
 */
void DcMotor_Init(void);
/*
 * Description :
 * Function responsible for State the DC Motor driver.
 */
void DcMotor_Rotate(DcMotor_State state);

#endif /* MOTOR_H_ */
