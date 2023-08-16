/***************************************************************************************/
/***************************************************************************************/
/**************************** Authors : Mohammed Elsayaad  *****************************/
/**************************** Date : 20 June,2023         ******************************/
/***************************************************************************************/
/***************************************************************************************/


#ifndef FACTORY_PRIVATE_H_
#define FACTORY_PRIVATE_H_

#define EEPROM_BASE			0x05
#define EEPROM_OFFSET		0x00

#define LCD_TEMP_OFFSET		5
#define LCD_HUMDT_OFFSET	9
#define LCD_GAS_OFFSET		4

#define  YES				1
#define  NO					0
static void Factory_voidMode(void);

static void Factory_voidEditProfil(LCD_Profile_Data_t *Copy_Temp,LCD_Profile_Data_t *Copy_Humdt,LCD_Profile_Data_t *Copy_Gas,APP_PANNEL_t Copy_CurrentPannel);

static void Factory_voidChoseProgram(void);
static void Factory_voidDisplayStrings(LCD_Profile_Data_t *Copy_Temp,LCD_Profile_Data_t *Copy_Humdt,LCD_Profile_Data_t *Copy_Gas);
static void Factory_voidSaveLogs(u8 *Copy_pu8String, u32 Copy_u32Data);
static void Factory_voidReadLogs(u8* Copy_pu8Log);
static void Factory_voidPanicModeOn(void);
static void Factory_voidDisplayLogs(void);
static void Factory_voidShowLogs(void);
#endif /* FACTORY_PRIVATE_H_ */