/***************************************************************************************/
/***************************************************************************************/
/**************************** Authors : Mohammed Elsayaad  *****************************/
/**************************** Date : 20 June,2023         ******************************/
/**************************** Description: 
This project is a comprehensive factory monitoring system that uses an ATmega32
micro controller, a 4x20 LCD display, and various sensors to monitor the state 
of a factory. The system is designed to detect and log hazardous conditions,
ensuring the safety and efficiency of the factory operations.			  
**************/
/***************************************************************************************/
/***************************************************************************************/

#define F_CPU	16000000UL
#include <util/delay.h>

#include "Factory_Monitor.h"
#include "Factory_Private.h"
/*********************Global Vars Definitions***************************/

volatile static APP_PANNEL_t APP_Pannel		   = MAIN;
static APP_PANNEL_t User_Program_Choice		   = MAIN;
volatile static u8 Factory_u8DisplayFlag	   = 0;

static u16	Log_u16Offset=0;

static u8* Log_pu8Temp = "Temp Was ";
static u8* Log_pu8Humd = "Humd Was ";
static u8* Log_pu8Gas = "Gas Was ";

static u16 Ultra_u16Dist  =0;
/************************************************************************************/
volatile static u8 Buffer[100];
volatile static u32 Counter;
static u16 Previous_u16Location=0;
/********************* External Functions Implementations***************************/



void APP_Factory_voidInit(void)
{
	/**** ADC Init ******/
	ADC_Init(ADC_VCCREF,DIV_128);
	/*********************/
	/**** EXTI Init ******/
	MCAL_EXTI_voidInitINT1(EXTI_FALLING_EDGE);
	MCAL_EXTI_voidSetCallBack(EXTI_INT1,&Factory_voidMode);
	/*********************/
	/**** Push butttons init ******/
	H_PushButton_Void_PushButtonInit(PUSH_BUTTON_0);
	//H_PushButton_Void_PushButtonInit(PUSH_BUTTON_1);
	H_PushButton_Void_PushButtonInit(PUSH_BUTTON_2);
	H_PushButton_Void_PushButtonInit(PUSH_BUTTON_3);
	/*********************/
	
	/**** LCD Init ******/
	H_Lcd_Void_LCDInit();
	/*********************/
	
	/**** Sensors Init ******/
	HAL_MQ_voidInit();
	LM35_Void_Init();
	/*********************/
	
	/**** UltraSonic Init ******/
	UltraSonic_Init();
	/*********************/
	
	/**** LED Init ******/
	H_LED_Void_LedInit(LED_RED);
	/*********************/
	
	/**** Buzzer Init ******/
	H_Buzzer_Void_BuzzerInit();
	/*********************/
	
	/**** Enable Interrupts ******/
	GIE();
	/*********************/
}

/**
 * @brief This function is used to control or monitor an environment based on temperature, humidity, and gas levels.
 * 
 * @details The function uses different profiles (Profile One, Profile Two, Profile Three) to set threshold values for these parameters. 
 * It runs in an infinite loop, where it checks the current panel (APP_Pannel) and performs different operations based on its value.
 * 
 * If the panel is set to 'MAIN', it allows the user to choose a program (Profile One, Two, or Three). Depending on the chosen program, 
 * it sets the threshold values for temperature, humidity, and gas. If the panel is set to 'PROFILE_ONE', it displays the values of 
 * temperature, humidity, and gas for Profile One and allows editing these values.
 *
 * @param void No parameters.
 *
 * @return void No return value.
 */
void APP_Factory_voidStart(void)
{


	u8 Local_u8OneTimeFlag		 =0;
	
	/*************************************************************************/
	u32 Local_u32ThresholdTemp   =0;
	u32 Local_u32ThresholdGas    =0;
	u32 Local_u32ThresholdHumdt  =0;
	/*************************************************************************/
	u32 Local_u32Temp			 =0;
	u32 Local_u32GasPrecentage	 =0;
	u32 Local_u32Humditiy		 =0;
	/**************************Profile one vars******************************/
	LCD_Profile_Data_t P1_Temp= {.Name ="Temp:",.Row=0,.Colum=0,.Value=35};
	LCD_Profile_Data_t P1_Humd= {.Name ="Humidity:",.Row=1,.Colum=0,.Value=70};
	LCD_Profile_Data_t P1_Gas= {.Name = "Gas:",.Row=2,.Colum=0,.Value=80};
	/**************************Profile Two vars*************************************/	
	LCD_Profile_Data_t P2_Temp= {.Name ="Temp:",.Row=0,.Colum=0,.Value=0};
	LCD_Profile_Data_t P2_Humd= {.Name ="Humidity:",.Row=1,.Colum=0,.Value=0};
	LCD_Profile_Data_t P2_Gas= {.Name = "Gas:",.Row=2,.Colum=0,.Value=0};
	/***************************Profile Three vars******************************/	
	LCD_Profile_Data_t P3_Temp= {.Name ="Temp:",.Row=0,.Colum=0,.Value=0};
	LCD_Profile_Data_t P3_Humd= {.Name ="Humidity:",.Row=1,.Colum=0,.Value=0};
	LCD_Profile_Data_t P3_Gas= {.Name = "Gas:",.Row=2,.Colum=0,.Value=0};
	for(;;) /*Infinity*/
	{
		switch (APP_Pannel)
		{
			case MAIN:
			/****************************/
			if((0 == Factory_u8DisplayFlag) && (MAIN == APP_Pannel))
			{
				H_Lcd_Void_LCDClear();
				_delay_ms(2);
				H_Lcd_Void_LCDGoTo(3,8);
				H_Lcd_Void_LCDWriteString("MAIN");
				Factory_u8DisplayFlag=1;
			}
			/********** Let User Choose Programm First ************/
			Factory_voidChoseProgram();
			/******************************************************/
			if(MAIN == User_Program_Choice)
			{
				/****
				* User didn't choose program to take threshold values from it so main won't start.
				***/
			}
			else
			{
				/*********************************/
				H_Lcd_Void_LCDClear();
				_delay_ms(2);
				H_Lcd_Void_LCDGoTo(3,8);
				H_Lcd_Void_LCDWriteString("MAIN");
				/*********************************/
				switch (User_Program_Choice)
				{
				case PROFILE_ONE:
					Local_u32ThresholdTemp = P1_Temp.Value;
					Local_u32ThresholdGas = P1_Gas.Value;
					Local_u32ThresholdHumdt	  = P1_Humd.Value;
					break;
				case PROFILE_TWO:
					Local_u32ThresholdTemp = P2_Temp.Value;
					Local_u32ThresholdGas = P2_Gas.Value;
					Local_u32ThresholdHumdt	  = P2_Humd.Value;
					break;
				case PROFILE_THREE:
					Local_u32ThresholdTemp = P3_Temp.Value;
					Local_u32ThresholdGas = P3_Gas.Value;
					Local_u32ThresholdHumdt	  = P3_Humd.Value;
					break;
				default:
					break;
				}
				APP_Pannel = NORMAL;
				/******************************************/
				
				/****************************************/
				//User_Program_Choice = MAIN;
			}
			break;
			case PROFILE_ONE:
			/****************************/
			if(0 == Factory_u8DisplayFlag && (PROFILE_ONE == APP_Pannel))
			{
				H_Lcd_Void_LCDClear();
				_delay_ms(2);
				H_Lcd_Void_LCDGoTo(3,8);
				H_Lcd_Void_LCDWriteString("P1");
				Factory_u8DisplayFlag=1;
			}
			/********************** Values(Temp, Humditity and Gas) Display  **************************/
			/***************************/
			H_Lcd_Void_LCDGoTo(P1_Temp.Row,P1_Temp.Colum);
			H_Lcd_Void_LCDWriteString(P1_Temp.Name);
			H_Lcd_Void_LCDWriteNumber(P1_Temp.Value);
			/**************************/
			H_Lcd_Void_LCDGoTo(P1_Humd.Row,P1_Humd.Colum);
			H_Lcd_Void_LCDWriteString(P1_Humd.Name);
			H_Lcd_Void_LCDWriteNumber(P1_Humd.Value);
			/**************************/
			H_Lcd_Void_LCDGoTo(P1_Gas.Row,P1_Gas.Colum);
			H_Lcd_Void_LCDWriteString(P1_Gas.Name);
			H_Lcd_Void_LCDWriteNumber(P1_Gas.Value);
			/********************** Call Input handler function  **************************/
			Factory_voidEditProfil(&P1_Temp,&P1_Humd,&P1_Gas,PROFILE_ONE);
			/*****************************************************************************/
			break;
			case PROFILE_TWO:
			/****************************/
			if(0 == Factory_u8DisplayFlag && (PROFILE_TWO == APP_Pannel))
			{
				H_Lcd_Void_LCDClear();
				_delay_ms(2);
				H_Lcd_Void_LCDGoTo(3,8);
				H_Lcd_Void_LCDWriteString("P2");
				Factory_u8DisplayFlag=1;
			}
			/********************** Values(Temp, Humditity and Gas) Display  **************************/
			/***************************/
			H_Lcd_Void_LCDGoTo(P2_Temp.Row,P2_Temp.Colum);
			H_Lcd_Void_LCDWriteString(P2_Temp.Name);
			H_Lcd_Void_LCDWriteNumber(P2_Temp.Value);
			/**************************/
			H_Lcd_Void_LCDGoTo(P2_Humd.Row,P2_Humd.Colum);
			H_Lcd_Void_LCDWriteString(P2_Humd.Name);
			H_Lcd_Void_LCDWriteNumber(P2_Humd.Value);
			/**************************/
			H_Lcd_Void_LCDGoTo(P2_Gas.Row,P2_Gas.Colum);
			H_Lcd_Void_LCDWriteString(P2_Gas.Name);
			H_Lcd_Void_LCDWriteNumber(P2_Gas.Value);
			/**************************/
			/********************** Call Input handler function  **************************/
			Factory_voidEditProfil(&P2_Temp,&P2_Humd,&P2_Gas,PROFILE_TWO);
			/*****************************************************************************/
			break;
			case PROFILE_THREE:
			/****************************/
			if(0 == Factory_u8DisplayFlag && (PROFILE_THREE == APP_Pannel))
			{
				H_Lcd_Void_LCDClear();
				_delay_ms(2);
				H_Lcd_Void_LCDGoTo(3,8);
				H_Lcd_Void_LCDWriteString("P3");
				Factory_u8DisplayFlag=1;
			}
			/********************** Values(Temp, Humditity and Gas) Display  **************************/
			/***************************/
			H_Lcd_Void_LCDGoTo(P3_Temp.Row,P3_Temp.Colum);
			H_Lcd_Void_LCDWriteString(P3_Temp.Name);
			H_Lcd_Void_LCDWriteNumber(P3_Temp.Value);
			/**************************/
			H_Lcd_Void_LCDGoTo(P3_Humd.Row,P3_Humd.Colum);
			H_Lcd_Void_LCDWriteString(P3_Humd.Name);
			H_Lcd_Void_LCDWriteNumber(P3_Humd.Value);
			/**************************/
			H_Lcd_Void_LCDGoTo(P3_Gas.Row,P3_Gas.Colum);
			H_Lcd_Void_LCDWriteString(P3_Gas.Name);
			H_Lcd_Void_LCDWriteNumber(P3_Gas.Value);
			/**************************/
			/********************** Call Input handler function  **************************/
			Factory_voidEditProfil(&P3_Temp,&P3_Humd,&P3_Gas,PROFILE_THREE);
			/*****************************************************************************/
			break;
			case PANIC:
				Factory_voidPanicModeOn();
				/*Display Logs?*/
				Factory_voidDisplayLogs();
				APP_Pannel = NORMAL;
				break;
			case NORMAL:
				Factory_voidDisplayStrings(&P1_Temp,&P1_Humd,&P1_Gas);
				while( NORMAL == APP_Pannel)
				{
					Local_u32Temp		   = LM35_getTemperature();
					/* Get reading of Gas
					*/
					Local_u32GasPrecentage = GetGasPercentage(ReadSensor(),LPG);;					
					
					/*********************** Display values on LCD **************************/
					
					H_Lcd_Void_LCDGoTo(P1_Temp.Row,P1_Temp.Colum + LCD_TEMP_OFFSET);
					H_Lcd_Void_LCDWriteNumber(Local_u32Temp);
					
					H_Lcd_Void_LCDGoTo(P1_Humd.Row,P1_Humd.Colum + LCD_HUMDT_OFFSET);
					H_Lcd_Void_LCDWriteNumber(Local_u32Humditiy);
					
					H_Lcd_Void_LCDGoTo(P1_Gas.Row,P1_Gas.Colum + LCD_GAS_OFFSET);
					H_Lcd_Void_LCDWriteNumber(Local_u32GasPrecentage);
					/**********************************************************************/
					if(Local_u32Temp >= Local_u32ThresholdTemp)
					{
						/*Save Log*/
						Factory_voidSaveLogs(Log_pu8Temp,Local_u32Temp);
						/***********************************************************************/
						MCAL_EXTI_voidDisableInterrupt(EXTI_INT1);	/*Disable Interrupt (EXTI) */
						H_LED_Void_LedOn(LED_RED);
						H_Buzzer_Void_BuzzerOn();
						/*Go TO Panic Mode*/
						APP_Pannel = PANIC;
					}
					else if(Local_u32GasPrecentage >= Local_u32ThresholdGas)
					{
						/*Save Log*/
						Factory_voidSaveLogs(Log_pu8Gas,Local_u32GasPrecentage);
						/*Go TO Panic Mode*/
						APP_Pannel = PANIC;
					}
					else if(Local_u32Humditiy >= Local_u32ThresholdHumdt)
					{
						/*Save Log*/
						Factory_voidSaveLogs(Log_pu8Humd,Local_u32Humditiy);
						/*Go TO Panic Mode*/
						APP_Pannel = PANIC;
					}
					else
					{
						/*Nothing*/
					}
				}
				break;
			default:
			/*Invalid*/
			break;
		}
	}
	
}








/*********************Static Functions Implementations***************************/

static void Factory_voidMode(void)
{
	APP_Pannel++;
	Factory_u8DisplayFlag = 0;
	if(APP_Pannel >= NUMBER_OF_PANNELS-2)
	{
		APP_Pannel = MAIN;
	}
}

static void Factory_voidEditProfil(LCD_Profile_Data_t *Copy_Temp,LCD_Profile_Data_t *Copy_Humdt,LCD_Profile_Data_t *Copy_Gas,APP_PANNEL_t Copy_CurrentPannel)
{
	APP_Values_t Local_u8ModifyCounter=APP_TEMP;
	while(Copy_CurrentPannel == APP_Pannel)
	{
		if(PUSH_BUTTON_PRESSED ==  H_PushButton_U8_PushButtonRead(PUSH_BUTTON_0))
		{
			/*Means user want to change in other value maybe (Temp, Humidity or Gas)*/
			Local_u8ModifyCounter++;
			Local_u8ModifyCounter= (Local_u8ModifyCounter)%NUMBER_OF_VALUES;
		}
		else
		{;}
		
		while(PUSH_BUTTON_PRESSED ==  H_PushButton_U8_PushButtonRead(PUSH_BUTTON_2))
		{
			/*Means user want to increase value (+)*/
			switch(Local_u8ModifyCounter)
			{
				case APP_TEMP:	(Copy_Temp->Value)++;
				H_Lcd_Void_LCDGoTo(Copy_Temp->Row,(Copy_Temp->Colum)+LCD_TEMP_OFFSET);
				H_Lcd_Void_LCDWriteNumber(Copy_Temp->Value);
				break;
				case APP_HUMDT: Copy_Humdt->Value++;
				H_Lcd_Void_LCDGoTo((Copy_Humdt->Row),(Copy_Humdt->Colum)+LCD_HUMDT_OFFSET);
				H_Lcd_Void_LCDWriteNumber(Copy_Humdt->Value);
				break;
				case APP_GAS:	Copy_Gas->Value++;
				H_Lcd_Void_LCDGoTo((Copy_Gas->Row),(Copy_Gas->Colum)+LCD_GAS_OFFSET);
				H_Lcd_Void_LCDWriteNumber(Copy_Gas->Value);
				break;
				default:
				/*Nothing*/
				break;
			}
		}
		
		
		
	}
}

static void Factory_voidChoseProgram(void)
{
	H_Lcd_Void_LCDGoTo(0,0);
	H_Lcd_Void_LCDWriteString("Press PB1 for First");
	H_Lcd_Void_LCDGoTo(1,0);
	H_Lcd_Void_LCDWriteString("Press PB2 for Second");
	H_Lcd_Void_LCDGoTo(2,0);
	H_Lcd_Void_LCDWriteString("Press PB3 for Third");
	while(MAIN == APP_Pannel)
	{
		if(PUSH_BUTTON_PRESSED == H_PushButton_U8_PushButtonRead(PUSH_BUTTON_0))
		{
			User_Program_Choice = PROFILE_ONE;
			break;
		}
		else if((PUSH_BUTTON_PRESSED == H_PushButton_U8_PushButtonRead(PUSH_BUTTON_2)))
		{
			User_Program_Choice = PROFILE_THREE;
			break;
		}
		else
		{
			;
		}
	}
}

static void Factory_voidDisplayStrings(LCD_Profile_Data_t *Copy_Temp,LCD_Profile_Data_t *Copy_Humdt,LCD_Profile_Data_t *Copy_Gas)
{
		H_Lcd_Void_LCDGoTo(Copy_Temp->Row,Copy_Temp->Colum);
		H_Lcd_Void_LCDWriteString(Copy_Temp->Name);
		/**************************/
		H_Lcd_Void_LCDGoTo(Copy_Humdt->Row,Copy_Humdt->Colum);
		H_Lcd_Void_LCDWriteString(Copy_Humdt->Name);
		/**************************/
		H_Lcd_Void_LCDGoTo(Copy_Gas->Row,Copy_Gas->Colum);
		H_Lcd_Void_LCDWriteString(Copy_Gas->Name);
}

static void Factory_voidSaveLogs(u8 *Copy_pu8String, u32 Copy_u32Data)
{
	u16 Local_u16EEPROMCounter=0;
	s16 Local_s16TempCounter=0;
	u8  Local_u8TempArray[5]={0};
	u16 Local_u16CurrentOffset=0;
	EEPROM_Receive(EEPROM_OFFSET,(u8*)&Log_u16Offset);
	if(0xff == Log_u16Offset)
	{
		Log_u16Offset =0;
	}
	else{;}
	Local_u16CurrentOffset = Log_u16Offset;
	/*****************************************************************************************************/
	for(Local_u16EEPROMCounter=0;Copy_pu8String[Local_u16EEPROMCounter] != '\0';Local_u16EEPROMCounter++)
	{
		EEPROM_Send(EEPROM_BASE+Local_u16CurrentOffset+Local_u16EEPROMCounter,Copy_pu8String[Local_u16EEPROMCounter]);
		Log_u16Offset++;
	}
	while(Copy_u32Data)
	{
		Local_u8TempArray[Local_s16TempCounter] = (Copy_u32Data % 10) +'0';
		Local_s16TempCounter++;
		Copy_u32Data /=10;
	}
	Local_s16TempCounter--;
	while(Local_s16TempCounter >= 0)
	{
		EEPROM_Send(EEPROM_BASE+Local_u16CurrentOffset+Local_u16EEPROMCounter,Local_u8TempArray[Local_s16TempCounter]);
		Local_s16TempCounter--;
		Local_u16EEPROMCounter++;
		Log_u16Offset++;
	}
	EEPROM_Send(EEPROM_BASE+Local_u16CurrentOffset+Local_u16EEPROMCounter,'\0');
	/***********Update Offset in EEPROM***************/
	Log_u16Offset = Local_u16CurrentOffset + Local_u16EEPROMCounter+1;
	if(255 <= Log_u16Offset)
	{
		Log_u16Offset =0;
	}
	else
	{
		;
	}
	EEPROM_Send(EEPROM_OFFSET,Log_u16Offset); /*Assumption that offset won't exceed 255 (Not real situation)*/
}

static void Factory_voidReadLogs(u8* Copy_pu8Log)
{
	u16 Local_u16Counter=0;
	u8 Local_u8Data=0;
	
	EEPROM_Receive(EEPROM_BASE+Previous_u16Location,&Local_u8Data);
	if(0xff == Local_u8Data)
	{
		/*No new Data exit*/
		Copy_pu8Log[0] = '\0';
	}
	else
	{
		while(Local_u8Data != '\0')
		{
			Copy_pu8Log[Local_u16Counter] = Local_u8Data;
			Buffer[Counter]=Local_u8Data;
			Counter++;
			Local_u16Counter++;
			EEPROM_Receive(EEPROM_BASE+Previous_u16Location+Local_u16Counter,&Local_u8Data);
		}
		/*****************************/
		Copy_pu8Log[Local_u16Counter] = Local_u8Data; /*Put null*/
		Previous_u16Location += (Local_u16Counter+1);
	}
	
}

static void Factory_voidPanicModeOn(void)
{
	Ultra_u16Dist = UltraSonic_Update();
	while(Ultra_u16Dist >= 10)
	{
		Ultra_u16Dist = UltraSonic_Update();
		//_delay_ms(600);
	}
	/*Someone is near*/
	H_LED_Void_LedOff(LED_RED);
	H_Buzzer_Void_BuzzerOff();
	while(Ultra_u16Dist < 10)
	{
		Ultra_u16Dist = UltraSonic_Update();
		//_delay_ms(600);
	}
	MCAL_EXTI_voidEnableInterrupt(EXTI_INT1,EXTI_FALLING_EDGE);
	APP_Pannel = NORMAL;
}

static void Factory_voidDisplayLogs(void)
{
	u8 Local_u8OptionFlag=0;
	H_Lcd_Void_LCDClear();
	_delay_ms(2);
	H_Lcd_Void_LCDGoTo(0,0);
	H_Lcd_Void_LCDWriteString("Display Logs?");
	H_Lcd_Void_LCDGoTo(1,0);
	H_Lcd_Void_LCDWriteString("Press PB4 for Yes");
	H_Lcd_Void_LCDGoTo(2,0);
	H_Lcd_Void_LCDWriteString("Press PB0 for No");
	/******** wait responce ************/
	for(;;) /*Infinity*/
	{
		if(PUSH_BUTTON_PRESSED ==  H_PushButton_U8_PushButtonRead(PUSH_BUTTON_0))
		{
			/*No*/
			Local_u8OptionFlag=0;
			break;
		}
		else
		{;}
		if(PUSH_BUTTON_PRESSED ==  H_PushButton_U8_PushButtonRead(PUSH_BUTTON_3))
		{
			/*Yes*/
			Local_u8OptionFlag=1;
			break;
		}
		else
		{;}
	}
	switch(Local_u8OptionFlag)
	{
	case YES:
		/*Call Show Logs*/
		H_Lcd_Void_LCDClear();
		_delay_ms(2);
		Factory_voidShowLogs();
		H_Lcd_Void_LCDClear();
		_delay_ms(2);
		break;
	case NO:
		/*Nothing*/
		H_Lcd_Void_LCDClear();
		_delay_ms(2);
		break;
	}
}

static void Factory_voidShowLogs(void)
{
	
	u8 Local_u8LCDLines[4][20] ={{'1'}}; 
	u8 *Local_pu8ArrayOfP[4] = {&Local_u8LCDLines[0],&Local_u8LCDLines[1],&Local_u8LCDLines[2],&Local_u8LCDLines[3]};
	u8 Local_u8PointerCounter=0;
	u32 Local_u32EEPROMCounter=0;
	u8 Local_u8LCDLine=0;
	while(PUSH_BUTTON_RELEASED == H_PushButton_U8_PushButtonRead(PUSH_BUTTON_3))
	{
		if(4 > Local_u8LCDLine)
		{
			Factory_voidReadLogs(Local_u8LCDLines[Local_u8LCDLine]);
		}
		else{;}
		
		/* Array is filled Display on LCD */
		if((4 == Local_u8LCDLine) || ('\0' == Local_u8LCDLines[Local_u8LCDLine][0]))
		{
			if((PUSH_BUTTON_PRESSED == H_PushButton_U8_PushButtonRead(PUSH_BUTTON_2)) && (Local_u8LCDLine >= 4))
			{
				/*Scroll Down*/
				/*1- Swap Pointers*/
				Local_pu8ArrayOfP[0] = Local_pu8ArrayOfP[1];
				Local_pu8ArrayOfP[1] = Local_pu8ArrayOfP[2];
				Local_pu8ArrayOfP[2] = Local_pu8ArrayOfP[3];
				Local_pu8ArrayOfP[3] = Local_u8LCDLines[Local_u8PointerCounter];
				Local_u8PointerCounter++;
				Local_u8PointerCounter = Local_u8PointerCounter %4;
				Factory_voidReadLogs(Local_pu8ArrayOfP[3]);
				if(('\0' == Local_u8LCDLines[3][0]))
				{
					/*Nothing to display more*/
				}
				else
				{
					H_Lcd_Void_LCDGoTo(0,0);
					H_Lcd_Void_LCDWriteString(Local_pu8ArrayOfP[0]);
					H_Lcd_Void_LCDGoTo(1,0);
					H_Lcd_Void_LCDWriteString(Local_pu8ArrayOfP[1]);
					H_Lcd_Void_LCDGoTo(2,0);
					H_Lcd_Void_LCDWriteString(Local_pu8ArrayOfP[2]);
					H_Lcd_Void_LCDGoTo(3,0);
					H_Lcd_Void_LCDWriteString(Local_pu8ArrayOfP[3]);
				}
			}
			
		}
		else
		{
			
			H_Lcd_Void_LCDGoTo(Local_u8LCDLine,0);
			H_Lcd_Void_LCDWriteString(Local_pu8ArrayOfP[Local_u8LCDLine]);
			Local_u8LCDLine++;
			
			
		}
		
		
	}
	
}