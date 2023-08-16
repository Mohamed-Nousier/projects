
#include "STD.h"
#include "DIO_Interface.h"
#include "LCD_Interface.h"
#include "Keypad_Interface.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "timers.h"

/************************************************************************/
/* Data type definitions                                                                     */
/************************************************************************/
typedef struct
{
	u8 firstVal;
	u8 Operator;
	u8 secondVal;
}equationMSG_t;

typedef struct
{
	equationMSG_t MSG;
	u8 updateFlag;
	u8 completeFlag;
}keypadBuffer_t;

typedef struct
{
	equationMSG_t equation;
	u8 resultVal;
}equationResultMSG_t;


#define KEYPAD_TIMEOUT     (10U)

TaskHandle_t KeyPadTask_Handle = NULL;
TaskHandle_t CalculatorTask_Handle  = NULL;
TaskHandle_t TimeTask_Handle  = NULL;
TaskHandle_t DisplayTask_Handle  = NULL;


xQueueHandle Keypad2Calc_Queue = NULL;
xQueueHandle Keypad2Disc_Queue = NULL;
xQueueHandle Calc2Dis_Queue = NULL;
xQueueHandle Time2Dis_Queue = NULL;


SemaphoreHandle_t KeyPad2LCD_ClearNotification = NULL;
SemaphoreHandle_t KeyPad2Timer_StartCounting = NULL;
SemaphoreHandle_t Timer2KeyPad_TenSecondsNotification = NULL;
SemaphoreHandle_t KeyPad2Timer_ResetCounting = NULL;


void System_Init(void);
void KeyPadTask_Func(void);
void CalculatorTask_Func(void);
void TimeTask_Func(void);
void DisplayTask_Func(void);
void Handle_Time(u8 *Time_String);


int main(void)
{
	System_Init();

	xTaskCreate(KeyPadTask_Func, "KeyPad_Task", 100, (void *)NULL,3, &KeyPadTask_Handle);
	xTaskCreate(CalculatorTask_Func, "Calculator_Task", 100, NULL, 2,&CalculatorTask_Handle);
	xTaskCreate(DisplayTask_Func, "Display_Task", 250, NULL,1, &DisplayTask_Handle);
	xTaskCreate(TimeTask_Func, "Time_Task", 250, NULL,4 ,&TimeTask_Handle);

	Keypad2Calc_Queue = xQueueCreate(1, sizeof(equationMSG_t));
	Keypad2Disc_Queue = xQueueCreate(1, sizeof(equationMSG_t));
	Calc2Dis_Queue    = xQueueCreate(1, sizeof(equationResultMSG_t));
	Time2Dis_Queue    = xQueueCreate(1,sizeof(u8)*20);

	KeyPad2LCD_ClearNotification =  xSemaphoreCreateBinary();
	KeyPad2Timer_StartCounting   =  xSemaphoreCreateBinary();
	Timer2KeyPad_TenSecondsNotification = xSemaphoreCreateBinary();
	KeyPad2Timer_ResetCounting   = xSemaphoreCreateBinary();

	vTaskStartScheduler();

	return 0;
}


void System_Init(void)
{
	H_Lcd_Void_LCDInit();
	H_KeyPad_Void_KeyPadInit();
}


void KeyPadTask_Func(void)
{
	equationMSG_t Input_equation;
	keypadBuffer_t keypadBuffer ;
    u8 Local_u8Input = KEYPAD_RELEASED ;
    u8 Local_u8inform = 0;
    u8 Local_u8Counter_sem = 0;

	while(1)
	{
		Local_u8Input = H_KeyPad_U8_KeyPadRead();

		/*
		 *1- take semaphore(KeyPad2Timer_StartCounting).
		 *2- check if 10 seconds have passed without any button pressed
		 *3- if pressed give semaphore(KeyPad2LCD_ClearNotification) to clear lcd
		 */
		if(Local_u8Counter_sem == 10)
		{
			xSemaphoreTake(KeyPad2Timer_StartCounting, 0);
			Local_u8Counter_sem = 0;
		}else{Local_u8Counter_sem++;}
		if(xSemaphoreTake(Timer2KeyPad_TenSecondsNotification,0) == pdTRUE)
		{
			keypadBuffer.completeFlag = '1';
			if(keypadBuffer.completeFlag == '1' && keypadBuffer.updateFlag == '1')
			{
				xSemaphoreGive(KeyPad2LCD_ClearNotification);
				keypadBuffer.completeFlag = '0';
				keypadBuffer.updateFlag = '0';
				Local_u8inform = 0;
			}else{;}
		}else{;}

		/**********************************************************************************/
		if (Local_u8Input != KEYPAD_RELEASED)
		{
			/*
			 * 1- starting by setting the updateFlag, to indicates that there is an entry from keypad
			 * 2- Giving semaphore(KeyPad2Timer_StartCounting) to start counting
			 */

			xSemaphoreGive(KeyPad2Timer_StartCounting);
			if (Local_u8inform == 0)
			{
				keypadBuffer.updateFlag = '1';

				Input_equation.firstVal =  Local_u8Input;
				Input_equation.Operator =  'N';
				Input_equation.secondVal = 'N';
				xQueueSend(Keypad2Disc_Queue, &Input_equation, 0);
			}else{;}
			if (Local_u8inform == 1)
			{
				keypadBuffer.updateFlag = '1';

				Input_equation.Operator = Local_u8Input;
				Input_equation.secondVal = 'N';
				xQueueSend(Keypad2Disc_Queue, &Input_equation, 0);
			}else{;}
			if (Local_u8inform == 2)
			{
				keypadBuffer.updateFlag = '1';

				Input_equation.secondVal = Local_u8Input;
				xQueueSend(Keypad2Disc_Queue, &Input_equation, 0);
			}else{;}
			if (Local_u8Input == '=')
			{
				keypadBuffer.updateFlag = '1';

				xQueueSend(Keypad2Calc_Queue,&Input_equation, 0);
			}else{;}

			Local_u8inform = (Local_u8inform + 1 ) %4;
		}
		else{;}
		vTaskDelay(101);

	}

}

void CalculatorTask_Func(void)
{
	equationMSG_t Cal2Receive;
	equationResultMSG_t  ResultMSG;

	while(1)
	{
		if(xQueueReceive(Keypad2Calc_Queue,&Cal2Receive, 0) == pdPASS)
		{
			switch(Cal2Receive.Operator)
			{
				case '+':
					ResultMSG.resultVal =  ((Cal2Receive.firstVal - '0') + (Cal2Receive.secondVal - '0'));break;
				case '-':
					ResultMSG.resultVal = (Cal2Receive.firstVal - '0') - (Cal2Receive.secondVal - '0');break;
				case '*':
					ResultMSG.resultVal = (Cal2Receive.firstVal - '0') * (Cal2Receive.secondVal - '0');break;
				case '/':
					ResultMSG.resultVal = (Cal2Receive.firstVal - '0') / (Cal2Receive.secondVal - '0');break;
				default:                                                                     break;
			}

			ResultMSG.equation.firstVal = Cal2Receive.firstVal;
			ResultMSG.equation.secondVal = Cal2Receive.secondVal;
			ResultMSG.equation.Operator = Cal2Receive.Operator;


			xQueueSend(Calc2Dis_Queue, &ResultMSG, 0);

		}else{;}

		vTaskDelay(152);
	}

}


void DisplayTask_Func(void)
{
	equationMSG_t LCD_equation;
	equationResultMSG_t LCD_equationRes;


	u8 Local_u8keypadBuffer[10];
	u8 Local_u8timeBuffer[15];
	u8 Local_u8calBuffer[10]={0};
	u8 Local_u8LCDsleep[] = "SLEEP MODE ON";

	while(1)
	{
			if(xSemaphoreTake(KeyPad2LCD_ClearNotification,0) == pdTRUE)
			{
				H_Lcd_Void_LCDGoTo(0,0);
				H_Lcd_Void_LCDWriteString(Local_u8LCDsleep);
				vTaskDelay(1000);
				H_Lcd_Void_LCDClear();
				vTaskDelay(500);
			}else{;}
			if(xQueueReceive(Time2Dis_Queue,Local_u8timeBuffer,0) == pdPASS)
			{
				H_Lcd_Void_LCDGoTo(1,0);
				H_Lcd_Void_LCDWriteString(Local_u8timeBuffer);
			}
			else
			{;}
			if (xQueueReceive(Keypad2Disc_Queue,&LCD_equation,0) == pdPASS)
			{
				Local_u8keypadBuffer[0] = LCD_equation.firstVal;if(Local_u8keypadBuffer[0]!=  'N'){H_Lcd_Void_LCDGoTo(0,0);H_Lcd_Void_LCDWriteCharacter(Local_u8keypadBuffer[0]);}
				Local_u8keypadBuffer[1] = LCD_equation.Operator;if(Local_u8keypadBuffer[1]!=  'N'){H_Lcd_Void_LCDGoTo(0,1);H_Lcd_Void_LCDWriteCharacter(Local_u8keypadBuffer[1]);}
				Local_u8keypadBuffer[2] = LCD_equation.secondVal;if(Local_u8keypadBuffer[2]!= 'N'){H_Lcd_Void_LCDGoTo(0,2);H_Lcd_Void_LCDWriteCharacter(Local_u8keypadBuffer[2]);}

			}else{;}

		   if(xQueueReceive(Calc2Dis_Queue,&LCD_equationRes,0) == pdPASS)
			{
			   Local_u8calBuffer[0] = LCD_equationRes.equation.firstVal;
			   Local_u8calBuffer[1] = LCD_equationRes.equation.Operator;
			   Local_u8calBuffer[2] = LCD_equationRes.equation.secondVal;
			   Local_u8calBuffer[3] = '=';

				H_Lcd_Void_LCDGoTo(0,0);
				H_Lcd_Void_LCDWriteString(Local_u8calBuffer);
			    H_Lcd_Void_LCDWriteNumber(LCD_equationRes.resultVal);

			}else{;}

			vTaskDelay(203);

	}

}


void TimeTask_Func(void)
{
	static u8 Local_u8Keypad2Timer_SetCounterON = 0;
	static u8 Local_u8Keypad2Timer_StartCount   = 0;
	u8 Time_String[20] = "Time: 00:00:00";
	while(1)
	{

		if (xSemaphoreTake(KeyPad2Timer_StartCounting, 0) == pdTRUE) //this semaphore will not be given unless it lcd is reseted or system
		{
			Local_u8Keypad2Timer_SetCounterON = 1;
			Local_u8Keypad2Timer_StartCount = 0;
			xSemaphoreGive(KeyPad2Timer_StartCounting);
		}else{;}

		if(Local_u8Keypad2Timer_SetCounterON == 1)
		{
			Local_u8Keypad2Timer_StartCount++;
			if (Local_u8Keypad2Timer_StartCount == (KEYPAD_TIMEOUT + 1))
			{
				xSemaphoreGive(Timer2KeyPad_TenSecondsNotification);
				Local_u8Keypad2Timer_SetCounterON = 0;
				Local_u8Keypad2Timer_StartCount = 0 ;
			}else{;}
		}else{;}

		/*************************************************************************************/
		xQueueSend(Time2Dis_Queue,Time_String,0);
		Handle_Time(Time_String);
		vTaskDelay(1000);
	}
}

void Handle_Time(u8 *Time_String)
{
	u8 sec_units = Time_String[13];
	u8 sec_tens =  Time_String[12];
	u8 min_units = Time_String[10];
	u8 min_tens =  Time_String[9];
	u8 hrs_units = Time_String[7];
	u8 hrs_tens =  Time_String[6];

	Time_String[13] = ++sec_units;
	if (sec_units == (u8)58)
	{
		Time_String[12] = ++sec_tens;
		Time_String[13] = '0';
		if (sec_tens == '6')
		{
			Time_String[10] = ++min_units;
			Time_String[12] = '0';
			Time_String[13] = '0';
			if (min_units == (u8)58)
			{
				Time_String[9] = ++min_tens;
				Time_String[10]=  '0';
				Time_String[12] = '0';
				Time_String[13]=  '0';

				if (min_tens == '6')
				{
					Time_String[7] = ++hrs_units;
					Time_String[9]  =  '0';
					Time_String[10] =  '0';
					Time_String[12]  = '0';
					Time_String[13] =  '0';
					if (hrs_units == (u8)58)
					{
						Time_String[6] = ++hrs_tens;
						Time_String[7] =  '0';
						Time_String[9]  = '0';
						Time_String[10] = '0';
						Time_String[12] = '0';
						Time_String[13] = '0';
					}
				}
			}
		}
	}


}








