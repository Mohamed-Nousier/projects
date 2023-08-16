/***************************************************************************************/
/***************************************************************************************/
/**************************** Authors : Mohammed Elsayaad & Marwan nousir **************/
/**************************             Mohammed shokr & mostafa yousry ****************/
/**************************** Date : 20 June,2023         ******************************/
/**************************** Describtion: APP Interface implementation file     *******/
/***************************************************************************************/
/***************************************************************************************/
#ifndef PROJECTS_INTERFACE_H_
#define PROJECTS_INTERFACE_H_



/************************** Lib includes ***************************/

#include "STD.h"
#include "BitMath.h"

/************************** ECU Layer includes ***************************/

#include "LED_Interface.h"
#include "Relay_Interface.h"
#include "LCD_Interface.h"
#include "PushButton_Interface.h"
#include "EEPROM_Interface.h"
#include "Buzzer_Interface.h"
#include "ICU_Interface.h"
#include "MOTOR_Interface.h"

/************************** MCAL includes ***************************/

#include "Timer_Interface.h"
#include "ADC_Interface.h"
#include "GIE_Interface.h"
#include	"UART.h"

/******************************* User Defined Macros *******************************/

/****** System tick in (ms) ********/
#define SYSTICK		5

/********************System states*********************/
#define HMI_STATE			0
#define WAITING_STATE		1
#define CCS_STATE			2
#define ULTRASONIC_STATE	3
#define BRAKE_STATE			4
#define LIGHT_STATE			5
#define ADDITIONAL_STATE	6


#define OK			1
#define NOK			0
#define FOR_EVER	1

/********* Push buttons pins *************/
#define BRAKE	PD7_PIN
#define BLINK	PD5_PIN
#define CRUISE	PD3_PIN

/*addresses of Flags in EEPROM*/
#define  WAITING_ADD       0x0A
#define  CRUISE_ADD        0x0B
#define  ULTRA_SONIC_ADD   0x0C
#define  PAGE_ADD           0

/*LDR Ranged Values*/
#define LIGHT				50
#define LIGHT_MAX			450
#define DARK				450
#define DARK_MAX			1100

/*Delay Times*/
#define DELAY_40ms			 8
#define DELAY_2000ms	     400
#define DELAY_2500ms		 500
#define DEALY_5000ms		 1000
#define DELAY_7500ms		 1500
#define DELAY_1000ms		 200

/*UART Actions*/
#define	   BRAKE_ON			'1'
#define    CRUISE_CONT_ON	'2'
#define    BLINK_ON			'3'

/*Ultrasonic max range*/
#define ULTRA_MAX	500.0 
#define ULTRA_MIN	2.0 
#define BRAKE_MIN	4.99

/*Diagnostic Macros*/
#define ULTRASONIC_ERROR_POSITION	0
#define ADC_ERROR_POSITION			1
#define DTC_NO_ERROR				0		
		
/*************** APIs Decleration ******************/

/************Main APIs***********************/
void CruiseControl_Void_Init(void);
void CruiseControl_Runnable(void);

/************Pedal APIS**********************/
void CruiseControl_Void_ACC_Pedal_ON(void);
void CruiseControl_Void_ACC_Pedal_OFF(void);
/************Waiting API**********************/
void CruiseControl_Void_BLK_OFF(void);
/************LCD APIS**********************/
void CruiseControl_Void_HMI_Write(void);
void CruiseControl_voidHMIUpdateUltra(f32 Copy_f32Distance);
void CruiseControl_Void_HMI_Write_Old(void);
/************CCS APIS**********************/
void CruiseControl_Void_CRS_SET(void);
void CruiseControl_Void_CRS_CLR(void);

/************************************************************************/
/* For reseting EEPROM locations Use (Write_Logs)                       */
/************************************************************************/
void Write_Logs(void);
void CruiseControl_EEPROM_Void_EEPROMWrite_BLNK_STAT(void);
void CruiseControl_EEPROM_Void_EEPROMWrite_CRSE_STAT(void);
void CruiseControl_EEPROM_Void_EEPROMWrite_ULTSNC_STAT(void);
void CruiseControl_Read_Logs(void);



/*********** ISR ***********/
void TIMER2_ISR(void);


#endif /* PROJECTS_INTERFACE_H_ */
