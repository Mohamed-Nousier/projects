#ifndef MOTOR_INTERFACE_H_
#define MOTOR_INTERFACE_H_
#include "DIO_Interface.h"


#define PWR PB3_PIN
#define DIR PB5_PIN
#define EN  PB6_PIN

void HAL_MOTOR_Init(void);

void HAL_MOTOR_CW(void);

void HAL_MOTOR_CCW(void);
void HAL_MOTOR_Stop(void);


#endif