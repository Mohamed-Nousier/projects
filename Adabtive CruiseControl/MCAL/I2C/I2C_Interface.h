/*
 * I2C_Interface.h
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#ifndef MCAL_I2C_I2C_INTERFACE_H_
#define MCAL_I2C_I2C_INTERFACE_H_

/*============================================================================================*/
/*   INCLUDES   */
/*============================================================================================*/
#include "STD.h"
#include "BitMath.h"

/*============================================================================================*/
/*   PROTTYPES   */
/*============================================================================================*/
void M_I2C_Void_I2CInit(void);
void M_I2C_Void_StartCondition(void);
void M_I2C_Void_StopCondition(void);
void M_I2C_Void_RepeatedStart(void);
void M_I2C_Void_SendSlaveAddressWrite(u8);
void M_I2C_Void_SendSlaveAddressRead(u8);
void M_I2C_Void_SendByte(u8);
u8   M_I2C_U8_ReadByte(void);

#endif /* MCAL_I2C_I2C_INTERFACE_H_ */
