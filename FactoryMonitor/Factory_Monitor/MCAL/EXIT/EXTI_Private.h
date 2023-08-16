/********************************************************************************************************/
/* @author : Mohammed Elsayaad																			*/
/* @file   : EXTI_Private.h																				*/
/* @Date   : 12 May																						*/
/********************************************************************************************************/

#ifndef EXTI_PRIVATE_H_
#define EXTI_PRIVATE_H_

/************************************************************************/
/*                         EXTI Registers.				        		*/
/************************************************************************/

#define INT0_VECT				__vector_1
#define INT1_VECT				__vector_2
#define INT2_VECT				__vector_3

//macro to unmasking global interrupt before save context
#define ISR_NOBLOCK		__attribute__ ((interrupt))
//macro to tell compile don't add any extra assembly codes (don't add context).
#define ISR_NAKED		__attribute__ ((naked))

//macro like func to init an isr function.
#define ISR(vector,...)   void vector(void) __attribute__ ((signal)); __VA_ARGS__ \
					      void vector(void)
//macros to enable and disable global interrupt
#define reti()	__asm__ __volatile__ ("reti" ::)
#define ret()	__asm__ __volatile__ ("ret" ::)

//Extern interrupt registers.
#define MCUCR	(*((volatile u8*)(0x55)))
//bits definition
//bits corresponding to sense control for int0
#define ISC00	0
#define ISC01	1
//bits corresponding to sense control for int1
#define ISC10	2
#define ISC11	3

#define MCUCSR  (*((volatile u8*)(0x54)))
//bit corresponding to sense control for int2
#define  ISC2	6

#define GICR	(*((volatile u8*)(0x5B)))
//bit for enable int1,2 and 0
#define INT1_ENABLE		7
#define INT0_ENABLE		6
#define INT2_ENABLE		5
#define GIFR	(*((volatile u8*)0x5A))
//bits coresbonding for interrupt flags of int0,1 and 2
#define INTF1	7
#define INTF0	6
#define INTF2	5

#endif