/********************************************************************************************************/
/* @author : Mohammed Elsayaad																			*/
/* @file   : EXTI_Program.c																				*/
/* @Date   : 12 May																						*/
/********************************************************************************************************/

#include "STD.h"
#include "BitMath.h"

#include "EXTI_Interface.h"
#include "EXTI_Private.h"
#include "EXTI_Cnfig.h"



/**************** Global Vars Definitions **************************/

static void (*PF_INT0)(void) = NULL_PTR;
static void (*PF_INT1)(void) = NULL_PTR;
static void (*PF_INT2)(void) = NULL_PTR;





/********************************************************************************************************/

void MCAL_EXTI_voidInitINT0(EXTI_SenseControl_t Copy_SenseType)
{
	/*Clear before set*/
	MCUCR &= (0xfd);
	/*Set Sense control*/
	MCUCR |= (Copy_SenseType);
	SET_BIT(GICR,INT0_ENABLE); /*Enable interrupt*/
}
void MCAL_EXTI_voidInitINT1(EXTI_SenseControl_t Copy_SenseType)
{
	/*Clear before set*/
	MCUCR &= (0xf3);
	/*Set Sense control*/
	MCUCR |= (Copy_SenseType<<2);
	SET_BIT(GICR,INT1_ENABLE); /*Enable interrupt*/
}
void MCAL_EXTI_voidInitINT2(EXTI_SenseControl_t Copy_SenseType)
{
	
	if((EXTI_FALLING_EDGE == Copy_SenseType))
	{
		SET_BIT(MCUCSR,ISC2);
		SET_BIT(GICR,INT2_ENABLE); /*Enable interrupt*/
	}
	else if((EXTI_RISING_EDGE == Copy_SenseType))
	{
		CLR_BIT(MCUCSR,ISC2);
		SET_BIT(GICR,INT2_ENABLE); /*Enable interrupt*/
	}
	else
	{
		/*Error Wrong option*/
	}
}

void MCAL_EXTI_voidEnableInterrupt(EXTI_INT_t Copy_Interrupt,EXTI_SenseControl_t Copy_SenseType)
{
	switch(Copy_Interrupt)
	{
	case EXTI_INT0:
		/*Clear before set*/
		MCUCR &= (0xfd);
		/*Set Sense control*/
		MCUCR |= (Copy_SenseType);
		SET_BIT(GICR,INT0_ENABLE); /*Enable interrupt*/
		break;
	case EXTI_INT1:
		/*Clear before set*/
		MCUCR &= (0xf3);
		/*Set Sense control*/
		MCUCR |= (Copy_SenseType<<2);
		SET_BIT(GICR,INT1_ENABLE); /*Enable interrupt*/
		break;
	case EXTI_INT2:
		if((EXTI_FALLING_EDGE == Copy_SenseType))
		{
			SET_BIT(MCUCSR,ISC2);
			SET_BIT(GICR,INT2_ENABLE); /*Enable interrupt*/
		}
		else if((EXTI_RISING_EDGE == Copy_SenseType))
		{
			CLR_BIT(MCUCSR,ISC2);
			SET_BIT(GICR,INT2_ENABLE); /*Enable interrupt*/
		}
		else
		{
			/*Error Wrong option*/
		}
		break;
	}
}
void MCAL_EXTI_voidDisableInterrupt(EXTI_INT_t Copy_Interrupt)
{
	switch(Copy_Interrupt)
	{
	case EXTI_INT0:
		CLR_BIT(GICR,INT0_ENABLE); /*Disable interrupt*/
		break;
	case EXTI_INT1:
		CLR_BIT(GICR,INT1_ENABLE); /*Disable interrupt*/
		break;
	case EXTI_INT2:
		CLR_BIT(GICR,INT2_ENABLE); /*Disable interrupt*/
		break;
	}
}

void MCAL_EXTI_voidSetCallBack(EXTI_INT_t Copy_Interrupt, void (*Copy_PF)(void))
{
	switch(Copy_Interrupt)
	{
	case EXTI_INT0:
		PF_INT0 = Copy_PF;
		break;
	case EXTI_INT1:
		PF_INT1 = Copy_PF;
		break;
	case EXTI_INT2:
		PF_INT2 = Copy_PF;
		break;
	}
}


/*************************** ISR *************************/

/*****INT0 ISR******/

ISR(INT0_VECT)
{
	if(NULL_PTR != PF_INT0)
	{
		PF_INT0();
	}
	else
	{
		/*Null pointer passed*/
	}
	
}
/*****INT1 ISR******/

ISR(INT1_VECT)
{
	if(NULL_PTR != PF_INT1)
	{
		PF_INT1();
	}
	else
	{
		/*Null pointer passed*/
	}
}
/*****INT2 ISR******/

ISR(INT2_VECT)
{
	if(NULL_PTR != PF_INT2)
	{
		PF_INT2();
	}
	else
	{
		/*Null pointer passed*/
	}
}



