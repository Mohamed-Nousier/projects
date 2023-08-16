

#include "DIO_Interface.h"

#include "MOTOR_Interface.h"







void HAL_MOTOR_Init(void)
{
	M_DIO_Void_SetPinDirection(PWR,OUTPUT);
	M_DIO_Void_SetPinDirection(DIR,OUTPUT);
	M_DIO_Void_SetPinDirection(EN,OUTPUT);
}


void HAL_MOTOR_CW(void)
{
	//M_DIO_Void_SetPinValue(PWR,HIGH);	
	M_DIO_Void_SetPinValue(DIR,HIGH);
	M_DIO_Void_SetPinValue(EN,HIGH);
}

void HAL_MOTOR_CCW(void)
{
	//M_DIO_Void_SetPinValue(PWR,HIGH);	
	M_DIO_Void_SetPinValue(DIR,LOW);
	M_DIO_Void_SetPinValue(EN,HIGH);
}

void HAL_MOTOR_Stop(void)
{
	//M_DIO_Void_SetPinValue(PWR,LOW);
	M_DIO_Void_SetPinValue(DIR,LOW);
	M_DIO_Void_SetPinValue(EN,LOW);
}