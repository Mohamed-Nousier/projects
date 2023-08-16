/*
 * LM35_Prog.c
 *
 * Created: 18-Jun-23 5:23:09 PM
 *  Author: Mostafayousre
 */ 


#include "LM35_Interface.h"
#include "LM35_Config.h"


void LM35_Void_Init(void)
{
	 M_DIO_Void_SetPinDirection(LM35_PIN ,INPUT);
	 M_DIO_Void_SetPinPullUpRes(LM35_PIN,DISABLE);
}



u8 LM35_getTemperature(void)
{		u16 lm35Voltage=0;
		u8 temperature = 0;
	
	    lm35Voltage = ADC_Read_MiliVolt(LM35_CHANNEL); // convert from adc lto voltage (milli volt)

	    // Temperature in Celsius
		/*10mv    1 c
			Y     ??
			? = 
		*/
	    temperature = lm35Voltage / 10;
	
	
	return temperature;
}










