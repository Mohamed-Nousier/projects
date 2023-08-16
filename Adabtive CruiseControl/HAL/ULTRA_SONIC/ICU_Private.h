/*
 * ICU_Private.h
 *
 * Created: 6/17/2023 12:13:26 AM
 *  Author: 20115
 */ 


#ifndef ICU_PRIVATE_H_
#define ICU_PRIVATE_H_


/*   REGISTERS   */
#define TCCR0_REG     *(volatile u8*)0x53
#define TIMSK_REG     *(volatile u8*)0x59
#define TIFR_REG     *(volatile u8*)0x58
#define TCNT0_REG     *(volatile u8*)0x52
#define OCR0_REG      *(volatile u8*)0x5C

#define TCNT1_REG      *(volatile u16*)0x4C
#define TCCR1A_REG    *(volatile u8*)0x4F
#define TCCR1B_REG    *(volatile u8*)0x4E
#define OCR1A_REG     *(volatile u16*)0x4A
#define ICR1_REG      *(volatile u16*)0x46

/*   TCCR0 BITS   */
#define WGM00_BIT                  6
#define COM01_BIT                  5
#define COM00_BIT                  4
#define WGM01_BIT                  3
#define CS02_BIT                   2
#define CS01_BIT                   1
#define CS00_BIT                   0

/*   TIMSK BITS   */
#define OCIE0_BIT                  1
#define TOIE0_BIT                  0

/*   TCCR1A BITS   */
#define COM1A1_BIT                 7
#define COM1A0_BIT                 6
#define WGM11_BIT                  1
#define WGM10_BIT                  0

/*   TCCR1B BITS   */
#define WGM13_BIT                  4
#define WGM12_BIT                  3
#define CS12_BIT                   2
#define CS11_BIT                   1
#define CS10_BIT                   0



#define    ON    1
#define    OFF   0

#define SEI()	__asm__ __volatile__ ("sei" ::)

#define  POLLING   0
#define  INTERRUPT  1
#endif /* ICU_PRIVATE_H_ */