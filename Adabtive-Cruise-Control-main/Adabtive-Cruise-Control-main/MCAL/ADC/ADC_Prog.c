/*
 * ADC_Prog.c
 *
 *  Created on: ???/???/????
 *      Author: dell
 */


/*--------------------------------------------------------------------------------------------*/
/*   INCLUDES   */
/*--------------------------------------------------------------------------------------------*/
#include "ADC_Config.h"
#include "ADC_Interface.h"
#include "ADC_Private.h"

/*--------------------------------------------------------------------------------------------*/
/*   FUNCTION BODY   */
/*--------------------------------------------------------------------------------------------*/
void M_ADC_Void_ADCInit(void)
{
#if   ADC_VREF   ==   AVCC
	SET_BIT(ADMUX_REG,REFS0_BIT);
	CLR_BIT(ADMUX_REG,REFS1_BIT);
#elif ADC_VREF   ==   AREF_PIN
	CLR_BIT(ADMUX_REG,REFS0_BIT);
	CLR_BIT(ADMUX_REG,REFS1_BIT);
#elif ADC_VREF   ==   _2V5
	SET_BIT(ADMUX_REG,REFS0_BIT);
	SET_BIT(ADMUX_REG,REFS1_BIT);
#endif
	/* TO SELECT RIGHT ADJUST */
	CLR_BIT(ADMUX_REG,ADLAR_BIT);
#if   ADC_PRESCALER_DV   ==   128
	SET_BIT(ADCSRA_REG,ADPS0_BIT);
	SET_BIT(ADCSRA_REG,ADPS1_BIT);
	SET_BIT(ADCSRA_REG,ADPS2_BIT);
#elif ADC_PRESCALER_DV   ==   64
	CLR_BIT(ADCSRA_REG,ADPS0_BIT);
	SET_BIT(ADCSRA_REG,ADPS1_BIT);
	SET_BIT(ADCSRA_REG,ADPS2_BIT);
#endif

	/* TO ENABLE AUTO TRIGGER MODE */
	SET_BIT(ADCSRA_REG,ADATE_BIT);
	/* TO ENABLE ADC CIRCUIT */
	SET_BIT(ADCSRA_REG,ADEN_BIT);
	/* TO START CONVERSION */
	SET_BIT(ADCSRA_REG,ADSC_BIT);
}

/*--------------------------------------------------------------------------------------------*/
/*   FUNCTION BODY   */
/*--------------------------------------------------------------------------------------------*/
void ADC_Start(u8 Copy_U8_ADCChannel)
{
	ADMUX_REG = (ADMUX_REG & ADMUX_MASK) | Copy_U8_ADCChannel;
}
u8  M_ADC_Void_ADCRead(u16 *Copy_PU16_Read)
{
		u8 Ret=0;
		if(GET_BIT(ADCSRA_REG,ADIF_BIT) == ADC_FINISH_CONVERTING)
		{
			Ret =1;
			*Copy_PU16_Read = ADCVALUE_REG;
		}
		else
		{
			Ret =0;
		}
	return Ret;
}


