/***************************************************************************************/
/***************************************************************************************/
/**************************** Authors : Mohammed Elsayaad  *****************************/
/**************************** Date : 20 June,2023         ******************************/
/***************************************************************************************/
/***************************************************************************************/


#ifndef FACTORY_MONITOR_H_
#define FACTORY_MONITOR_H_


#include "LED_Interface.h"
#include "LCD_Interface.h"
#include "PushButton_Interface.h"
#include "Timer_Interface.h"
#include "ADC_Interface.h"
#include "DIO_Interface.h"
#include "EXTI_Interface.h"
#include "MQ2_Interface.h"
#include "Ultra_Sonic.h"
#include "LM35_Interface.h"
#include "Buzzer_Interface.h"
#include "EEPROM_interface.h"
/**************************/

/**
 * @brief Enumerates the different panels in the factory monitoring application.
 *
 * This enumeration is used to identify the different panels in the factory monitoring 
 * application. Each panel represents a different state or mode of operation.
 */
typedef enum
{
	MAIN=0,          /**< The main panel. */
	PROFILE_ONE,     /**< The first profile panel. */
	PROFILE_TWO,     /**< The second profile panel. */
	PROFILE_THREE,   /**< The third profile panel. */
	PANIC,           /**< The panic panel, activated during hazardous conditions. */
	NORMAL,          /**< The normal operation panel. */
	NUMBER_OF_PANNELS/**< The total number of panels. */
}APP_PANNEL_t;

/**
 * @brief Enumerates the different types of sensor values in the factory monitoring application.
 *
 * This enumeration is used to identify the different types of sensor values that are 
 * monitored by the factory monitoring application.
 */
typedef enum
{
	APP_TEMP=0,      /**< Temperature value from the temperature sensor. */
	APP_HUMDT,       /**< Humidity value from the humidity sensor. */
	APP_GAS,         /**< Gas concentration value from the gas sensor. */
	NUMBER_OF_VALUES /**< The total number of sensor values. */
}APP_Values_t;

/**
 * @brief Represents a data entry for a profile on the LCD display.
 *
 * This structure is used to store information about a data entry for a profile on 
 * the LCD display in the factory monitoring application.
 */
typedef struct  
{
	u8 *Name;  /**< The name of the data entry. */
	u8 Row;    /**< The row on the LCD where the data entry is displayed. */
	u8 Colum;  /**< The column on the LCD where the data entry is displayed. */
	u32 Value; /**< The value of the data entry. */
}LCD_Profile_Data_t;


/**************************/

/**
 * @brief Initializes the factory monitoring application.
 *
 * This function is responsible for initializing the factory monitoring application. 
 * It sets up the ATmega32 micro controller, the 4x20 LCD display, and the various sensors 
 * (temperature, gas, and humidity). It should be called before any other functions in 
 * the application are used.
 *
 * @return void
 */
void APP_Factory_voidInit(void);
/**
 * @brief Starts the factory monitoring application.
 *
 * This function starts the factory monitoring application. It begins collecting data from 
 * the sensors and displaying it on the LCD. If a hazard is detected, it logs the sensor 
 * data in the EEPROM. This function should be called after APP_Factory_voidInit().
 *
 * @return void
 */
void APP_Factory_voidStart(void);

#endif /* FACTORY_MONITOR_H_ */