/*
 * Ultra_Sonic.h
 *
 * Created: 6/21/2023 12:34:01 PM
 *  Author: 20115
 */ 


#ifndef ULTRA_SONIC_H_
#define ULTRA_SONIC_H_

#include "STD.h"
#include "BitMath.h"
#include "DIO_Interface.h"
#include "Timer_Interface.h"
#include "UltraSonic_Private.h"


#define  TRIGGER_PIN  PC0_PIN


void UltraSonic_Init(void);
f32 UltraSonic_Update(void);


#endif /* ULTRA_SONIC_H_ */