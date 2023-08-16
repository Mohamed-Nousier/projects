/*
 * EEPROM_Prog.c
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

/*============================================================================================*/
/*   INCLUDES   */
/*============================================================================================*/
#include "EEPROM_Interface.h"
#include "EEPROM_Private.h"

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void H_EEPROM_Void_EEPROMInit(void)
{
	M_I2C_Void_I2CInit();
	_delay_ms(500);
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void H_EEPROM_Void_EEPROMWrite(u8 Copy_U8_PageNumber,u8 Copy_U8_ByteNumber,u8 Copy_U8_Data)
{
	M_I2C_Void_StartCondition();
	M_I2C_Void_SendSlaveAddressWrite(EEPROM_MASK | Copy_U8_PageNumber);
	M_I2C_Void_SendByte(Copy_U8_ByteNumber);         /*   TO SEND BYTE NUMBER   */
	M_I2C_Void_SendByte(Copy_U8_Data);               /*   TO SEND DATA          */
	M_I2C_Void_StopCondition();
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
u8   H_EEPROM_U8_EEPROMRead(u8 Copy_U8_PageNumber,u8 Copy_U8_ByteNumber)
{
	u8 Local_U8_Data = 0;
	M_I2C_Void_StartCondition();
	M_I2C_Void_SendSlaveAddressWrite(EEPROM_MASK | Copy_U8_PageNumber);
	M_I2C_Void_SendByte(Copy_U8_ByteNumber);         /*   TO SEND BYTE NUMBER   */
	M_I2C_Void_RepeatedStart();
	M_I2C_Void_SendSlaveAddressRead(EEPROM_MASK | Copy_U8_PageNumber);
	Local_U8_Data = M_I2C_U8_ReadByte();
	M_I2C_Void_StopCondition();
	return Local_U8_Data;
}
