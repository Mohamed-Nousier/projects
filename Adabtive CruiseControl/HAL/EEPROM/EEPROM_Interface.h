/*
 * EEPROM_Interface.h
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#ifndef HAL_EEPROM_EEPROM_INTERFACE_H_
#define HAL_EEPROM_EEPROM_INTERFACE_H_

/*============================================================================================*/
/*   INCLUDES   */
/*============================================================================================*/
#include "STD.h"
#include "I2C_Interface.h"
#include <util/delay.h>


#if (EEPROM_PAGE_NUMBER > 3)
#error the maximum page number is 3
#endif

/*============================================================================================*/
/*   PROTOTYPES   */
/*============================================================================================*/
void H_EEPROM_Void_EEPROMInit(void);
void H_EEPROM_Void_EEPROMWrite(u8,u8,u8);
u8   H_EEPROM_U8_EEPROMRead(u8,u8);

#endif /* HAL_EEPROM_EEPROM_INTERFACE_H_ */
