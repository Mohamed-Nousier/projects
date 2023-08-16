/********************************************************************************************************/
/* @author : Mohammed Elsayaad																			*/
/* @file   : EXTI_Interface.h																			*/
/* @Date   : 12 May																						*/
/********************************************************************************************************/

#ifndef EXTI_INTERFACE_H_
#define EXTI_INTERFACE_H_

/***************** Macro Definition ********************/
#define sei()	__asm__ __volatile__ ("sei" ::)
#define cli()	__asm__ __volatile__ ("cli" ::)

#define GIE()		sei()
#define GID()		cli()

typedef enum
{
	EXTI_LOW_LEVEL=0,
	EXTI_ANY_CHANGE,
	EXTI_FALLING_EDGE,
	EXTI_RISING_EDGE
}EXTI_SenseControl_t;

typedef enum
{
	EXTI_INT2=5,
	EXTI_INT0,
	EXTI_INT1,
	
}EXTI_INT_t;


/************************************ External APIs Decleration ***************************************************/

extern void MCAL_EXTI_voidInitINT0(EXTI_SenseControl_t Copy_SenseType);
extern void MCAL_EXTI_voidInitINT1(EXTI_SenseControl_t Copy_SenseType);
extern void MCAL_EXTI_voidInitINT2(EXTI_SenseControl_t Copy_SenseType);

extern void MCAL_EXTI_voidEnableInterrupt(EXTI_INT_t Copy_Interrupt,EXTI_SenseControl_t Copy_SenseType);
extern void MCAL_EXTI_voidDisableInterrupt(EXTI_INT_t Copy_Interrupt);

extern void MCAL_EXTI_voidSetCallBack(EXTI_INT_t Copy_Interrupt, void (*Copy_PF)(void));


#endif