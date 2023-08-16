/*
 * ADC_Interface.h
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#ifndef MCAL_ADC_ADC_INTERFACE_H_
#define MCAL_ADC_ADC_INTERFACE_H_

/*--------------------------------------------------------------------------------------------*/
/*   INCLUDES   */
/*--------------------------------------------------------------------------------------------*/
#include "STD.h"
#include "BitMath.h"

/*--------------------------------------------------------------------------------------------*/
/*   DEFINES   */
/*--------------------------------------------------------------------------------------------*/
#define ADC_CHANNEL_0              0
#define ADC_CHANNEL_1              1
#define ADC_CHANNEL_2              2
#define ADC_CHANNEL_3              3
#define ADC_CHANNEL_4              4
#define ADC_CHANNEL_5              5
#define ADC_CHANNEL_6              6
#define ADC_CHANNEL_7              7

/*--------------------------------------------------------------------------------------------*/
/*   PROTOTYPES   */
/*--------------------------------------------------------------------------------------------*/
void M_ADC_Void_ADCInit(void);
u8  M_ADC_Void_ADCRead(u16 *Copy_PU16_Read);
void ADC_Start(u8 Copy_U8_ADCChannel);

#endif /* MCAL_ADC_ADC_INTERFACE_H_ */
