/*
 * LDR_Private.h
 *
 *  Created on: ???/???/????
 *      Author: dell
 */

#ifndef HAL_LCD_LCD_PRIVATE_H_
#define HAL_LCD_LCD_PRIVATE_H_

/*--------------------------------------------------------------------------------------------*/
/*   DEFINES   */
/*--------------------------------------------------------------------------------------------*/
#define _8_BIT_MODE                   8
#define _4_BIT_MODE                   4
#define LCD_8_BIT_MODE_COMMAND        0x38
#define LCD_DISPLAY_ON_COMMAND        0x0C
#define LCD_RETURN_HOME_COMMAND       0x02
#define LCD_4_BIT_MODE_COMMAND_1      0x33
#define LCD_4_BIT_MODE_COMMAND_2      0x32
#define LCD_4_BIT_MODE_COMMAND_3      0x28
#define LCD_CLEAR                     0x01
#define LCD_R0_COMMAND                0x80
#define LCD_R1_COMMAND                0xC0
#define LCD_R2_COMMAND                0x94
#define LCD_R3_COMMAND                0xD4
#define NULL                          '\0'
#define  ACTIVATE  1
#define  DISABLE   0

#endif /* HAL_LCD_LCD_PRIVATE_H_ */
