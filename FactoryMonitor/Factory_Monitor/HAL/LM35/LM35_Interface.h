/*
 * LM35_Interface.h
 *
 * Created: 18-Jun-23 5:22:06 PM
 *  Author: Mostafayousre
 */ 



#ifndef HAL_LM35_Interface_H_
#define HAL_LM35_Interface_H_

#include "STD.h"
#include "ADC_Interface.h"
#include "DIO_Interface.h"


void LM35_Void_Init(void);
u8 LM35_getTemperature(void);


#endif /* HAL_LM35_Interface_H_ */





