/*
 * ICU_Prog.c
 *
 * Created: 6/17/2023 12:12:28 AM
 *  Author: 20115
 */ 

#include "ICU_Interface.h"




static volatile u16 TIMER_OVF = 0;
static volatile u8 string[10];
static volatile u32 count;
static volatile f32 distance;




void Ultra_Sonic_Init(void)
{
	M_DIO_Void_SetPinDirection(PB0_PIN,OUTPUT);
	//LCD_Init();
	//LCD_SendStringPos("ULTRASONIC", 1, 0);
	SET_BIT(TIMSK_REG, 2); 
	
}


f32 Ultra_Sonic_Update(void)
{
	u32 Count=0;
	/*****************************************************************/
		TCNT1_REG = 0;         /* Clear Timer counter */ 
		TCCR1B_REG = 0x41;     /* Capture on rising edge, No prescaler*/
		SET_BIT(TIFR_REG, 5);  /* Clear ICP flag (Input Capture flag) */
		SET_BIT(TIFR_REG, 2);  /* Clear Timer Overflow flag */
		/******************************************************************/
		M_DIO_Void_SetPinValue(PB0_PIN,HIGH);
		_delay_us(10);
		M_DIO_Void_SetPinValue(PB0_PIN,LOW);
	/*****************************************************************/	
	/*Calculate width of Echo by Input Capture (ICP) */
		while (GET_BIT(TIFR_REG, 5) == 0 && (65535 >= Count))
		{Count++;} /* Wait for rising edge */
		TCNT1_REG = 0;                      /* Clear Timer counter */
		TCCR1B_REG = 0x01;                  /* Capture on falling edge, No prescaler */
		SET_BIT(TIFR_REG, 5);               /* Clear ICP flag (Input Capture flag) */
		SET_BIT(TIFR_REG, 2);               /* Clear Timer Overflow flag */
		TIMER_OVF = 0;                      /* Clear Timer overflow count */
		Count=0;
	/****************************************************************/	
		while (GET_BIT(TIFR_REG, 5) == 0 && (65535 >= Count))
		{Count++;}; /* Wait for Falling edge */
	/****************************************************************/
	count = ICR1_REG + (65535 * TIMER_OVF);	/* Take count */
	distance = (double)(count / 466.47)/2;
	return distance;
	
}





void __vector_9(void)__attribute__((signal)); //ovf_INT
void __vector_9(void)
{
	TIMER_OVF++;
}