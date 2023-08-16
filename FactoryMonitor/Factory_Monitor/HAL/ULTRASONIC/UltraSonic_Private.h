/*
 * UltraSonic_Private.h
 *
 * Created: 6/21/2023 1:04:10 PM
 *  Author: 20115
 */ 


#ifndef ULTRASONIC_PRIVATE_H_
#define ULTRASONIC_PRIVATE_H_

#define TCCR0_REG     *(volatile u8*)0x53
#define TIMSK_REG     *(volatile u8*)0x59
#define TIFR_REG     *(volatile u8*)0x58
#define TCNT0_REG     *(volatile u8*)0x52
#define OCR0_REG      *(volatile u8*)0x5C
#define TCNT1_REG     *(volatile u16*)0x4C
#define TCCR1A_REG    *(volatile u8*)0x4F
#define TCCR1B_REG    *(volatile u8*)0x4E
#define OCR1A_REG     *(volatile u16*)0x4A
#define ICR1_REG      *(volatile u16*)0x46



#endif /* ULTRASONIC_PRIVATE_H_ */