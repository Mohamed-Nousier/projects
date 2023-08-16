#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_
#include "STD.h"
#include "BitMath.h"


typedef enum
{
	ADC_VCCREF,
	ADC_AREF,
	ADC_256REF
}ADC_VoltageRef_Type;

typedef enum
{
	DIV_2=1,
	DIV_4,
	DIV_8,
	DIV_16,
	DIV_32,
	DIV_64,
	DIV_128
}ADC_PREScaller_Type;

typedef enum
{
	CH0,
	CH1,
	CH2,
	CH3,
	CH4,
	CH5,
	CH6,
	CH7
}ADC_Chanels_Type;

extern void ADC_Init(ADC_VoltageRef_Type vref,ADC_PREScaller_Type prescaller);
extern u16	ADC_Read(ADC_Chanels_Type ch);
extern u16  ADC_Read_MiliVolt(ADC_Chanels_Type ch);
#endif /* ADC_INTERFACE_H_ */