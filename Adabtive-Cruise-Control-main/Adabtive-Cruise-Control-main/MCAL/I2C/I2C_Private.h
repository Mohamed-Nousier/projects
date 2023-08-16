/*
 * I2C_Private.h
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#ifndef MCAL_I2C_I2C_PRIVATE_H_
#define MCAL_I2C_I2C_PRIVATE_H_

/*============================================================================================*/
/*   DEFINES   */
/*============================================================================================*/
#define I2C_DISABLE                                   0
#define I2C_ENABLE                                    1
#define I2C_STILL_PROCESSING                          0

#define I2C_START_CONDITION_HAS_BEEN_TRANSMITTED      0x08
#define I2C_SEND_SLAVE_ADD_WRITE_ACK_REC              0x18
#define I2C_SEND_SLAVE_ADD_READ_ACK_REC               0x40
#define I2C_REPEATED_START_COND                       0x10
#define I2C_DATA_BYTE_TRANSMITTED_ACK_REC             0x28

/*   REGISTERS   */
#define TWAR_REG      *(volatile u8*)0x22
#define TWBR_REG      *(volatile u8*)0x20
#define TWCR_REG      *(volatile u8*)0x56
#define TWDR_REG      *(volatile u8*)0x23
#define TWSR_REG      *(volatile u8*)0x21

/*   TWCR BITS   */
#define TWINT_BIT                   7
#define TWSTA_BIT                   5
#define TWSTO_BIT                   4
#define TWEN_BIT                    2

#endif /* MCAL_I2C_I2C_PRIVATE_H_ */
