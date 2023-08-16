/*
 * ICU_Interface.h
 *
 * Created: 6/17/2023 12:13:00 AM
 *  Author: 20115
 */ 


#ifndef ICU_INTERFACE_H_
#define ICU_INTERFACE_H_

#include <stdlib.h>
#define  F_CPU  16000000UL
#include <util/delay.h>

#include "STD.h"
#include "BitMath.h"

//#include "DIO_Interface.h"
#include "DIO_Interface.h"
#include "ICU_Private.h"
#include "ICU_Config.h"
//#include "LCD_Interface.h"
#include "LED_Interface.h"

void Ultra_Sonic_Init(void);
void Ultra_Sonic_Start(void);
f32 Ultra_Sonic_Update(void);


#endif /* ICU_INTERFACE_H_ */