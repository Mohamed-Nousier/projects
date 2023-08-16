/*
 * I2C_Prog.c
 *
 *  Created on: ???/???/????
 *      Author: dell
 */


/*============================================================================================*/
/*   INCLUDES   */
/*============================================================================================*/
#include "I2C_Config.h"
#include "I2C_Interface.h"
#include "I2C_Private.h"

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void M_I2C_Void_I2CInit(void)
{

	TWBR_REG = (((I2C_F_CPU * 1000000) / ((u32)I2C_F_SCL * 1000)) - 16) / 2;

	//TWAR_REG = (I2C_SLAVE_ADDRESS << 1) | I2C_GENERAL_CALL;

	/* TO ENABLE 12C CIRCUIT */
	SET_BIT(TWCR_REG,TWEN_BIT);
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void M_I2C_Void_StartCondition(void)
{
	SET_BIT(TWCR_REG,TWSTA_BIT);
	SET_BIT(TWCR_REG,TWINT_BIT);          /* TO CLEAR THE FLAG */
	while(GET_BIT(TWCR_REG,TWINT_BIT) == I2C_STILL_PROCESSING);
	while((TWSR_REG & 0xF8) != I2C_START_CONDITION_HAS_BEEN_TRANSMITTED);
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void M_I2C_Void_StopCondition(void)
{
	SET_BIT(TWCR_REG,TWSTO_BIT);
	SET_BIT(TWCR_REG,TWINT_BIT);          /* TO CLEAR THE FLAG */
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void M_I2C_Void_RepeatedStart(void)
{
	SET_BIT(TWCR_REG,TWSTA_BIT);
	SET_BIT(TWCR_REG,TWINT_BIT);          /* TO CLEAR THE FLAG */
	while(GET_BIT(TWCR_REG,TWINT_BIT) == I2C_STILL_PROCESSING);
	while((TWSR_REG & 0xF8) != I2C_REPEATED_START_COND);
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void M_I2C_Void_SendSlaveAddressWrite(u8 Copy_U8_SlaveAddress)
{
	TWDR_REG = (Copy_U8_SlaveAddress << 1);
	SET_BIT(TWCR_REG,TWINT_BIT);          /* TO CLEAR THE FLAG */
	while(GET_BIT(TWCR_REG,TWINT_BIT) == I2C_STILL_PROCESSING);
	while((TWSR_REG & 0xF8) != I2C_SEND_SLAVE_ADD_WRITE_ACK_REC);
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void M_I2C_Void_SendSlaveAddressRead(u8 Copy_U8_SlaveAddress)
{
	TWDR_REG = (Copy_U8_SlaveAddress << 1) | 0x01;
	CLR_BIT(TWCR_REG,TWSTA_BIT);
	SET_BIT(TWCR_REG,TWINT_BIT);          /* TO CLEAR THE FLAG */
	while(GET_BIT(TWCR_REG,TWINT_BIT) == I2C_STILL_PROCESSING);
	while((TWSR_REG & 0xF8) != I2C_SEND_SLAVE_ADD_READ_ACK_REC);
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
void M_I2C_Void_SendByte(u8 Copy_U8_Data)
{
	TWDR_REG = Copy_U8_Data;
	CLR_BIT(TWCR_REG,TWSTA_BIT);
	SET_BIT(TWCR_REG,TWINT_BIT);          /* TO CLEAR THE FLAG */
	while(GET_BIT(TWCR_REG,TWINT_BIT) == I2C_STILL_PROCESSING);
	while((TWSR_REG & 0xF8) != I2C_DATA_BYTE_TRANSMITTED_ACK_REC);
}

/*============================================================================================*/
/*   FUNCTION BODY   */
/*============================================================================================*/
u8   M_I2C_U8_ReadByte(void)
{
	CLR_BIT(TWCR_REG,TWSTA_BIT);
	SET_BIT(TWCR_REG,TWINT_BIT);          /* TO CLEAR THE FLAG */
	while(GET_BIT(TWCR_REG,TWINT_BIT) == I2C_STILL_PROCESSING);
	//while((TWSR_REG & 0xF8) != I2C_READ_BYTE);
	return TWDR_REG;
}
