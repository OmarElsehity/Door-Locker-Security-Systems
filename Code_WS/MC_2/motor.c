/******************************************************************************
 *
 * Module: DC MOTOR
 *
 * File Name: motor.c
 *
 * Description: source file for the DC MOTOR driver
 *
 * Author: Omar Elsehity
 *
 *******************************************************************************/

#include "motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the DC Motor driver.
 */
void DcMotor_Init(void){
	/* motor pin1 output*/
	GPIO_setupPinDirection(MOTOR_PORT, MOTOR_PIN1, PIN_OUTPUT);

	/* motor pin2 output*/
	GPIO_setupPinDirection(MOTOR_PORT, MOTOR_PIN2, PIN_OUTPUT);

	/* STOP MOTOR AT START*/
	GPIO_writePin(MOTOR_PORT, MOTOR_PIN1, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT, MOTOR_PIN2, LOGIC_LOW);
}

/*
 * Description :
 * Function responsible for State the DC Motor driver.
 */
void DcMotor_Rotate(DcMotor_State state){

	switch(state){

	case 0 :
		/* STOP MOTOR*/
		GPIO_writePin(MOTOR_PORT,MOTOR_PIN1,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT,MOTOR_PIN2,LOGIC_LOW);
		break;
	case 1:
		/* CW Rotation*/
		GPIO_writePin(MOTOR_PORT,MOTOR_PIN1,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT,MOTOR_PIN2,LOGIC_LOW);
		break;
	case 2:
		/* ACW Rotation*/
		GPIO_writePin(MOTOR_PORT,MOTOR_PIN1,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT,MOTOR_PIN2,LOGIC_HIGH);
		break;

	}
}
