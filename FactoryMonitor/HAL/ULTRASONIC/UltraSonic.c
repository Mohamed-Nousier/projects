/*
 * UltraSonic.c
 *
 * Created: 6/21/2023 12:33:47 PM
 *  Author: 20115
 */ 

#include "Ultra_Sonic.h"



#define  F_CPU  16000000UL
#include <util/delay.h>

static volatile u32 TimerOverflow = 0;
static volatile f64 count = 0;
static volatile f64 distacne = 0;

void UltraSonic_Init(void)
{
	M_DIO_Void_SetPinDirection(TRIGGER_PIN, OUTPUT);
	//SET_BIT(TIMSK_REG,2); /* Enable Timer1 overflow interrupts */
	TCCR1A_REG = 0;
}

f32 UltraSonic_Update(void)
{
		
	u32 Local_u32TimeOut=0;

	/**********************************************/
	
	TCNT1_REG = 0;
	TCCR1B_REG = 0x41;		/*Input capture rising*/
	SET_BIT(TIFR_REG,5);
	//SET_BIT(TIFR_REG,2);
	
	M_DIO_Void_SetPinValue(TRIGGER_PIN, HIGH);
	_delay_us(10);
	M_DIO_Void_SetPinValue(TRIGGER_PIN, LOW);
	/*********************************************/
	while(GET_BIT(TIFR_REG, 5) == 0 && (65535 != Local_u32TimeOut))
	{
		Local_u32TimeOut++;
		_delay_us(10);
	}
	TCNT1_REG = 0;
	TCCR1B_REG = 0x01; /*Falling edge*/
	SET_BIT(TIFR_REG,5);
	//SET_BIT(TIFR_REG,2);
	while(GET_BIT(TIFR_REG, 5) == 0 && (65535 != Local_u32TimeOut))
	{
		Local_u32TimeOut++;
		_delay_us(10);
	}
	
	/*************************************************/
	TimerOverflow  = 0;
	/**************************************************/
	//while(GET_BIT(TIFR_REG, 5) == 0);
	count = ICR1_REG + (65535 * TimerOverflow);
	distacne = (double)(count / 466.47)/2;
	
	return distacne;
}