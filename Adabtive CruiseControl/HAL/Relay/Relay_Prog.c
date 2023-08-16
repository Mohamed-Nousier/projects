/*
 * Relay_Prog.c
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#include "Relay_Interface.h"


void H_Relay_Void_RelayInit(void) //relay2 PA7
{
	M_DIO_Void_SetPinDirection(PA7_PIN, OUTPUT);
	M_DIO_Void_SetPinDirection(PC7_PIN, OUTPUT);
}
void H_Relay_Void_RelayOn(void)
{
	M_DIO_Void_SetPinValue(PA7_PIN, HIGH);
	M_DIO_Void_SetPinValue(PC7_PIN, HIGH);
}
void H_Relay_Void_RelayOff(void)
{
	M_DIO_Void_SetPinValue(PA7_PIN, LOW);
	M_DIO_Void_SetPinValue(PC7_PIN, LOW);
}
