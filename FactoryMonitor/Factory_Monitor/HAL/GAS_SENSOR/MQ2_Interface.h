/*
 * MQ2_Interface.h
 *
 * Created: 4/27/2023 12:04:08 AM
 *  Author: moham
 */ 


#ifndef MQ2_INTERFACE_H_
#define MQ2_INTERFACE_H_
 

#include <math.h>
#include "STD.h"
#include "ADC_Interface.h"

/************************************************/
#define RL_VALUE			(10)     //define the load resistance on the board, in kilo ohms
#define RO_CLEAN_AIR_FACTOR (9.83)  //(Sensor resistance in clean air)/RO,which is derived from the chart in datasheet
#define LPG					(0)         // Gas identity no.
#define SMOKE				(1)
#define  Ro					19.91f
#define MQ_CHANNEL			CH1
/***********************************************/

void HAL_MQ_voidInit(void);
u32  GetPercentage(f32, f32 *);
u32 GetGasPercentage(f32 , u32);
float ReadSensor();
float ResistanceCalculation(u32);


#endif /* MQ2_INTERFACE_H_ */