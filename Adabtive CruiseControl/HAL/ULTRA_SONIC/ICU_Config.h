/*
 * ICU_Config.h
 *
 * Created: 6/17/2023 12:13:41 AM
 *  Author: 20115
 */ 


#ifndef ICU_CONFIG_H_
#define ICU_CONFIG_H_

#include "DIO_Interface.h"

/********** ULTRA SONIC TRIGGER PIN ********/
#define  ULTRA_SONIC_PORT              DIO_PORTC
#define  ULTRA_SONIC_PIN_TRIGGER       Pin0   

#define  ULTRA_SONIC     ON
/********************************************/

#define  TIMER_PRESCALER        1024
/**********/
/***OPTION:
		1
		8
		64
		256
		1024
*********/  

/*******************************************/

#define  ICU_RUN_MODE    POLLING
/**********/
/***OPTION:
		POLLING
		INTERRUPT
*/
/*********/ 

#endif /* ICU_CONFIG_H_ */