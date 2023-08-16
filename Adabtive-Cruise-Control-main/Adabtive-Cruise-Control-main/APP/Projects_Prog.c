/***************************************************************************************/
/***************************************************************************************/
/**************************** Authors : Mohammed Elsayaad & Marwan nousir **************/
/**************************             Mohammed shokr & mostafa yousry ****************/
/**************************** Date : 20 June,2023         ******************************/
/**************************** Describtion: APP C implementation file     ***************/
/***************************************************************************************/
/***************************************************************************************/


/************************************************************************/
/* Idea: Using timer2 to generate external exception (IRQ) every 5ms, system will look at each case every 5ms 
			(Time Triggered System), so every state has its own (ON) time and (OFF) time. 
			The (Off) time depends on execution time of other cases.
			For LCD and EEPROM it's a huge headache to check them every 5ms, so another counters were used to
			trigger them every specific amount of time.
			SYSTICK_5MS : is our SysTick.
*/
/************************************************************************/


/****************************** Include Section ******************************************/

#include "Projects_Interface.h"

/**************** Global Variables definitions ******************************/

/***************Static Volatile Vars*********************/

static volatile u8  SYSTICK_5MS			 =0; /*SysTick*/
static volatile u16 CCS_Pressed_Time_2ms =0; /*To measure 2000ms pressing time for CCS button*/
static volatile u16 UltraSonic_Time		 =0; /*To trigger ultrasonic every specific amount of time*/
static volatile u16 ADC_Time			 =0; /*To trigger ADC every specific amount of time*/
static volatile u8  Crs_PB_Pressed		 =0; /*Flag to indicate that CCS button was pressed and start to increment CCS_Pressed_Time_2ms */
static volatile u16 UART_Check_Time		 =0; /*To check UART every specific amount of time*/
static volatile u16 Waiting_Check_Time	 =0; /*To count time for waiting state (used in ISR)*/

/***************Static Vars*********************/
static u8 Crs_SYS_On		=0;	   /*Flag for CCS is on or off*/
static u8 Blink_OFF			=1;    /*Flag for Waiting is on or off*/
static f32 Ultra_f32Dest	=0.0f; /*Storing reading of ultrasonic in it (Global as it's used in EEPROM read and write functions)*/
static u8 Prev_u8Dest		=0;    /*Storing previous distance measured by ultrasonic*/


/***************************** Implementation ***********************************/

void CruiseControl_Void_Init(void)
{
	
	/*Buzzer*/
	H_Buzzer_Void_BuzzerInit();
	/*LEDs*/
	H_LED_Void_LedInit(LED_YEL);
	H_LED_Void_LedInit(LED_BLU);
	H_LED_Void_LedInit(LED_GRN);
	H_LED_Void_LedInit(LED_RED);
	/*PB*/
	H_PushButton_Void_PushButtonInit(PUSH_BUTTON_0);
	H_PushButton_Void_PushButtonInit(PUSH_BUTTON_1);
	H_PushButton_Void_PushButtonInit(PUSH_BUTTON_2);
	H_PushButton_Void_PushButtonInit(PUSH_BUTTON_3);
	/*LCD*/
	H_Lcd_Void_LCDInit();
	/*Relay*/
	H_Relay_Void_RelayInit();
	/*EEPROM*/
	H_EEPROM_Void_EEPROMInit();
	/*Motor*/
	HAL_MOTOR_Init();
	/*PWM*/
	M_Timer_Void_PWMInit(TIMER0_CHANNEL);
	M_Timer_Void_PWMSetDutyCycle(TIMER0_CHANNEL,0.0);
	M_Timer_Void_PWMStart(TIMER0_CHANNEL);
	/*Ultra*/
	Ultra_Sonic_Init();
	/*Timer2*/
	Timer2_Init();
	Timer2_SetCallBack(TIMER2_ISR);	
	M_Timer_Void_Timer2SetTime(SYSTICK);
	/*ADC*/
	M_ADC_Void_ADCInit();
	/*UART*/
	Uart_Init();
	/*EEPROM*/
	CruiseControl_Read_Logs();
	/*Bad practice*/
	if(1 == Crs_SYS_On)
	{
		CruiseControl_Void_CRS_SET();
	}
	
}

void CruiseControl_Runnable(void)
{
	u8 Local_u8OneTimeFlag		=0;				/*Used in HMI_State to print only one time at start of system*/
	u8 Local_u8WaitingTempFlag	=0;				/*Used in W8 state to make press does its action only one time instead of (while or busy w8 style)*/
	u8 Local_u8HMICounter		=0;				/*Used to write on LCD every specific amount of counts (ticks)*/
	u8 Local_u8CCSTempFlag		=0;				/*Same purpose as (Local_u8WaitingTempFlag)*/
	f32 Local_f32Duty			=0.0f;			/*Sending value of PWM duty cycle through it in (Ultrasonic State)*/
	u8 Local_u8BrakeFlag		=0;				/*Flag for indicating that user want to activate brake system or not*/
	u8 Local_u8TempBrake		=0;				/*Same purpose as (Local_u8WaitingTempFlag)*/
	u8 Local_u8AdcState			=0;				/*Flag for indicating that ADC finished previous sample and ready to start again*/
	u16 Local_u16AdcRead		=0;				/*Store ADC read in it (Obvious from the name)*/
	u16 Local_u16EEPROMCount	=0;				/*Used to write on EEPROM every specific amount of counts (ticks) in (Ultrasonic state)*/
	u8 Local_u8UART				='0';			/*Saving UART received message in it*/
	u8 Local_u8PrevUART			=1;				/*Saving previous UART received message in it*/
	u8 Local_u8DTCFlag			=DTC_NO_ERROR;	/*Flag indicate health state of system (0 = DTC_NO_ERROR) */
	u16 Local_u16DTCTime		=0;				/*Used to write on LCD every specific amount of counts (ticks) in (Diagnostic)*/
	
	/*Start System*/
	M_Timer_Void_Timer2Start();
	M_GIE_Void_GlobalInterruptEnable();
	
	while(FOR_EVER)
	{
		switch (SYSTICK_5MS)
	{
		case HMI_STATE:
		//HMI
		if(NOK == Local_u8OneTimeFlag)
		{
			/*To handle first time power on sys*/
			CruiseControl_Void_HMI_Write();
			CruiseControl_Void_HMI_Write_Old();
			H_Lcd_Void_LCDGoTo(1,0);
			H_Lcd_Void_LCDWriteString("Diagnostic: OK     ");
			Local_u8OneTimeFlag =1;
		}
		else{;}
		Local_u8HMICounter++;
		if(DELAY_1000ms == Local_u8HMICounter)
		{
			CruiseControl_Void_HMI_Write();
			Local_u8HMICounter=0;
		}
		else{;}
		break;
		
		case WAITING_STATE:
		//BLK
			if(PUSH_BUTTON_PRESSED==M_DIO_Void_GetPinValue(BLINK))
			{
				if(NOK == Local_u8WaitingTempFlag)
				{
					Blink_OFF ^= 0x01;
					Local_u8WaitingTempFlag = 1;
					CruiseControl_EEPROM_Void_EEPROMWrite_BLNK_STAT();
				}
				else{;}
			}
			else
			{
				if(OK==Blink_OFF)
				{
					CruiseControl_Void_BLK_OFF();
				}
				else
				{;}
				Local_u8WaitingTempFlag =0;
			}
		break;
		
		case CCS_STATE:
		//CCS
			if(PUSH_BUTTON_PRESSED == M_DIO_Void_GetPinValue(CRUISE))
			{
				if(NOK == Local_u8CCSTempFlag)
				{
					Crs_PB_Pressed=1;
					Local_u8CCSTempFlag=1;
					Crs_SYS_On ^= 0x01;
					CruiseControl_EEPROM_Void_EEPROMWrite_CRSE_STAT();
				}
				else{;}
			
			}
			else
			{
				Local_u8CCSTempFlag=0;
				CCS_Pressed_Time_2ms =0;
				Crs_PB_Pressed=0;
			}
			if(DELAY_2000ms <= CCS_Pressed_Time_2ms)
			{
				if(OK == Crs_SYS_On)
				{
					CruiseControl_Void_CRS_SET();
				}
				else
				{
					CruiseControl_Void_CRS_CLR();
				}
				CCS_Pressed_Time_2ms =0;
			}
			else
			{;}
		break;

		case ULTRASONIC_STATE:
			/*Ultrasonic*/
			if(DELAY_2500ms <= UltraSonic_Time)
			{
				Ultra_f32Dest = Ultra_Sonic_Update();
				if(ULTRA_MAX < Ultra_f32Dest)
				{
					Ultra_f32Dest = ULTRA_MAX;
					Local_u8DTCFlag |= (1<<ULTRASONIC_ERROR_POSITION);
				}
				else if(ULTRA_MIN > Ultra_f32Dest)
				{
					Ultra_f32Dest = ULTRA_MIN;
				}
				else
				{
					Local_u8DTCFlag &= ~(1<<ULTRASONIC_ERROR_POSITION);
				}
				if(OK==Crs_SYS_On)
				{
					
					if(Ultra_f32Dest >= 5.01f)
					{
						HAL_MOTOR_CW();
						CruiseControl_Void_ACC_Pedal_ON();
						/*Equation map dest to duty where 5cm = 20% and 20cm = 100%
						* Duty = Slope*(Dest-5)+ 20%.
						*/
						Local_f32Duty = ((80.0*(Ultra_f32Dest-5.0f))/95.0) + 20.0;
						M_Timer_Void_PWMSetDutyCycle(TIMER0_CHANNEL,Local_f32Duty);
					}
					else
					{
						HAL_MOTOR_Stop();
						M_Timer_Void_PWMSetDutyCycle(TIMER0_CHANNEL,0.0);
						CruiseControl_Void_ACC_Pedal_OFF();
					}
					
				}
				else
				{
					CruiseControl_Void_ACC_Pedal_OFF();
				}
				/****************** Update Reading and saving current value ************************/
				CruiseControl_voidHMIUpdateUltra(Ultra_f32Dest);
				Local_u16EEPROMCount++;
				if (Local_u16EEPROMCount > DELAY_40ms)
				{
					CruiseControl_EEPROM_Void_EEPROMWrite_ULTSNC_STAT();
					Local_u16EEPROMCount = 0;
				}
				UltraSonic_Time =0;
			}
			else{;}
		break;
		case BRAKE_STATE:
			
			if(PUSH_BUTTON_PRESSED == M_DIO_Void_GetPinValue(BRAKE))
			{
				/*Turn on Brake sys*/
				if(NOK == Local_u8TempBrake)
				{
					Local_u8BrakeFlag ^= 0x01;
					Local_u8TempBrake =1;
				}
				else{;}
			}
			else
			{
				Local_u8TempBrake =0;
			}									
			
			if(OK == Local_u8BrakeFlag)
			{
				/*Brake On*/
				if(BRAKE_MIN >= Ultra_f32Dest)
				{
					H_LED_Void_LedOn(LED_BLU);	
				}
				else
				{
					H_LED_Void_LedOff(LED_BLU);	
				}
			}
			else
			{
				H_LED_Void_LedOff(LED_BLU);
			}
			
			break;
		case LIGHT_STATE:
			/*Day or Night*/
			if(DELAY_7500ms <= ADC_Time)
			{
				if(1 == Local_u8AdcState)
				{
					ADC_Start(ADC_CHANNEL_0);
					Local_u8AdcState=0;
				}
				else
				{
					Local_u8AdcState = M_ADC_Void_ADCRead(&Local_u16AdcRead);
					if(1 == Local_u8AdcState)
					{
						ADC_Time=0;
						if((Local_u16AdcRead >= LIGHT) && (Local_u16AdcRead <LIGHT_MAX))
						{
							H_Lcd_Void_LCDGoTo(0,11);
							H_Lcd_Void_LCDWriteString("DAY   ");
							Local_u8DTCFlag &= ~(1<<ADC_ERROR_POSITION);
						}
						else if((Local_u16AdcRead >= DARK) && (Local_u16AdcRead <DARK_MAX))
						{
							H_Lcd_Void_LCDGoTo(0,11);
							H_Lcd_Void_LCDWriteString("NIGHT");
							Local_u8DTCFlag &= ~(1<<ADC_ERROR_POSITION);
						}
						else
						{
							Local_u8DTCFlag |= (1<<ADC_ERROR_POSITION);
						}
					}
					else
					{
						;
					}
				}
			}
			else
			{
				;
			}
			
			break;
		case ADDITIONAL_STATE:
			/*Diagnostic*/
			Local_u16DTCTime++;
			
			if(DELAY_2500ms <= Local_u16DTCTime)
			{
				if(DTC_NO_ERROR < Local_u8DTCFlag)
				{
					H_Lcd_Void_LCDGoTo(1,0);
					H_Lcd_Void_LCDWriteString("Diagnostic: Error");
				}
				else if(DTC_NO_ERROR == Local_u8DTCFlag)
				{
					H_Lcd_Void_LCDGoTo(1,0);
					H_Lcd_Void_LCDWriteString("Diagnostic: OK     ");
				}
				else{;}
				Local_u16DTCTime=0;
			}
			
			/*UART*/
			if(DELAY_1000ms <= UART_Check_Time)
			{
				Local_u8PrevUART=Local_u8UART;	
				Local_u8UART = Uart_Rxchar(); 
				if(BRAKE_ON == Local_u8UART || CRUISE_CONT_ON == Local_u8UART || BLINK_ON == Local_u8UART)
				{
					
					if(CRUISE_CONT_ON==Local_u8UART && Local_u8UART != Local_u8PrevUART)
					{
						Crs_SYS_On ^= 0x01;
						CruiseControl_EEPROM_Void_EEPROMWrite_CRSE_STAT();
						if(OK == Crs_SYS_On)
						{
							CruiseControl_Void_CRS_SET();
						}
						else
						{
							CruiseControl_Void_CRS_CLR();
						}
						
					}
					else if(BRAKE_ON==Local_u8UART && Local_u8UART != Local_u8PrevUART)
					{
						/*Turn on Brake sys*/
						Local_u8BrakeFlag ^= 0x01;
					}
					else if(BLINK_ON==Local_u8UART && Local_u8UART != Local_u8PrevUART)
					{
						Blink_OFF ^= 0x01;
						CruiseControl_EEPROM_Void_EEPROMWrite_BLNK_STAT();
					}
					else{;}
				}
				else
				{
					Local_u8PrevUART=Local_u8UART;
					
				}
				UART_Check_Time=0;
			}
			SYSTICK_5MS=0;
			break;
	}
	}
	
}


/******************************** Sub APIs **************************************/

/**CRS APIs****/
void CruiseControl_Void_CRS_SET(void)
{
	/*	RED LED ON	*/
	H_LED_Void_LedOn(LED_RED);
	
	/*	BUZZER ONCE	*/
	H_Buzzer_Void_BuzzerOnce();

}

void CruiseControl_Void_CRS_CLR(void)
{
	/*	prevState 	*/
		H_LED_Void_LedOff(LED_RED);
		H_Buzzer_Void_BuzzerOnce();
}

/**PEDAL APIs****/

void CruiseControl_Void_ACC_Pedal_ON(void)
{
		H_LED_Void_LedOn(LED_YEL);
}

void CruiseControl_Void_ACC_Pedal_OFF(void)
{
	
		H_LED_Void_LedOff(LED_YEL);
}

/**BLINK(Waiting) APIs****/
void CruiseControl_Void_BLK_OFF(void)
{
	H_LED_Void_LedOff(LED_GRN);
	H_Relay_Void_RelayOff();
}

/**LCD APIs****/
void CruiseControl_Void_HMI_Write(void)
{
	static u8 One_Entery_FLag=0;
	static u8 Prev_State=1;
	u8 *CRS = "CRS:";
	u8 *Distance ="Distance:";
	u8 *Distance_old ="Old_Dista:";
	u8 *status_off="OFF";
	u8 *status_on="ON ";
	
	if( 0 == One_Entery_FLag)
	{
		H_Lcd_Void_LCDGoTo(0,0);
		H_Lcd_Void_LCDWriteString(CRS);
		H_Lcd_Void_LCDGoTo(2,0);
		H_Lcd_Void_LCDWriteString(Distance);
		H_Lcd_Void_LCDGoTo(3,0);
		H_Lcd_Void_LCDWriteString(Distance_old);
		H_Lcd_Void_LCDGoTo(3,14);
		H_Lcd_Void_LCDWriteString("Cm");
		One_Entery_FLag =1;	
	}
	else
	{
		/*Nothing*/;
	}
	
	
	if (1 == Crs_SYS_On)
	{
		if(0 == Prev_State)
		{
			H_Lcd_Void_LCDGoTo(0,4);
			H_Lcd_Void_LCDWriteString(status_on);
			Prev_State =1;	
		}
		else
		{
			;
		}
		
		
	}
	else
	{
		if(1 == Prev_State)
		{
			H_Lcd_Void_LCDGoTo(0,4);
			H_Lcd_Void_LCDWriteString(status_off);
			Prev_State =0;
		}
		else
		{
			;
		}
			
	}
	
}

void CruiseControl_Void_HMI_Write_Old(void)
{	
	H_Lcd_Void_LCDGoTo(3,10);
	H_Lcd_Void_LCDWriteNumber(Prev_u8Dest);
}

void CruiseControl_voidHMIUpdateUltra(f32 Copy_f32Distance)
{
		H_Lcd_Void_LCDGoTo(2,10);
		H_Lcd_Void_LCDWriteNumber((u16)Copy_f32Distance);
		H_Lcd_Void_LCDWriteString("Cm");
		H_Lcd_Void_LCDWriteString("   ");
}


/**EEPROM APIs****/

void Write_Logs(void )
{
	
	H_EEPROM_Void_EEPROMWrite(PAGE_ADD, WAITING_ADD, '1');
	_delay_ms(1000);
	H_EEPROM_Void_EEPROMWrite(PAGE_ADD, CRUISE_ADD, '2');
	_delay_ms(1000);
	H_EEPROM_Void_EEPROMWrite(PAGE_ADD, ULTRA_SONIC_ADD,0xFF);
	_delay_ms(1000);
	
}

void CruiseControl_Read_Logs(void)
{
	u8 Local_u8_Blink_off = 0;
	u8 Local_u8_crs_sys_on = 0;
	u8 Local_u8_dest      = 0;
	
	 Local_u8_Blink_off = H_EEPROM_U8_EEPROMRead(PAGE_ADD, WAITING_ADD);
	 _delay_ms(1000);
	 Local_u8_crs_sys_on= H_EEPROM_U8_EEPROMRead(PAGE_ADD, CRUISE_ADD);
	 _delay_ms(1000);
	 Local_u8_dest    = H_EEPROM_U8_EEPROMRead(PAGE_ADD, ULTRA_SONIC_ADD);
	if ((u8)0 == Local_u8_Blink_off || (u8)1 == Local_u8_Blink_off)
	{
		Blink_OFF=Local_u8_Blink_off;
	}
	else
	{
		;
	}
	/*****************/
	if ((u8) 0 == Local_u8_crs_sys_on || (u8)1 == Local_u8_crs_sys_on)
	{
		Crs_SYS_On =Local_u8_crs_sys_on;
	}
	else
	{	
		;
	}
	/********************************/
	if (Local_u8_dest == 0xFF)
	{
		//Prev_u8Dest=Local_u8_dest;
	}
	else
	{
		Prev_u8Dest=Local_u8_dest;
	}
	
	
	
}

void CruiseControl_EEPROM_Void_EEPROMWrite_BLNK_STAT(void)
{
	H_EEPROM_Void_EEPROMWrite(PAGE_ADD, WAITING_ADD, Blink_OFF);
}


void CruiseControl_EEPROM_Void_EEPROMWrite_CRSE_STAT(void)
{
	H_EEPROM_Void_EEPROMWrite(PAGE_ADD, CRUISE_ADD, Crs_SYS_On);
}


void CruiseControl_EEPROM_Void_EEPROMWrite_ULTSNC_STAT(void)
{
	H_EEPROM_Void_EEPROMWrite(PAGE_ADD, ULTRA_SONIC_ADD,(u8)Ultra_f32Dest);
}






/********************************ISR****************************************/

void TIMER2_ISR(void)
{
	SYSTICK_5MS++;
	UltraSonic_Time++;
	ADC_Time++;
	UART_Check_Time++;
	if(NOK == Blink_OFF)
	{
		Waiting_Check_Time++;
		if(Waiting_Check_Time <= DELAY_2500ms)
		{
			H_LED_Void_LedOn(LED_GRN);
			H_Relay_Void_RelayOn();
		}
		else if(Waiting_Check_Time <= DEALY_5000ms)
		{
			H_LED_Void_LedOff(LED_GRN);
			H_Relay_Void_RelayOff();
		}
		else
		{
			Waiting_Check_Time=0;
		}
	}
	else
	{
		Waiting_Check_Time=0;
	}
	if(1==Crs_PB_Pressed)
	{
		CCS_Pressed_Time_2ms++;
	}
	else{;}
}
