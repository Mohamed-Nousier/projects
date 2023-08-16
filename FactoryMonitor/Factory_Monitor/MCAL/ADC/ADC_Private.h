#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_

/************************************************************************/
/*                         ADC Registers.				        		*/
/************************************************************************/
//ADC Lower byte register
#define ADCL	(*((volatile u8 *)(0x24)))
//ADC Higher byte register
#define ADCH	(*((volatile u8 *)(0x25)))
#define ADCR	(*((volatile u16 *)(0x24)))
//ADC Control and Status Register
#define ADCSRA	(*((volatile u8 *)(0x26)))
//Set to 1 to enable ADC.
#define ADEN	7
//set to 1 to start conversion.
#define ADSC	6
//set 1 to enble auto trigger conversion.
#define ADATE	5
//This bit is set when an ADC conversion completes and the Data Registers are updated.
#define ADIF	4
//set to one and the I-bit in SREG is set, the ADC Conversion Completed Interrupt is activated.
#define ADIE	3
//These bits determine the division factor between the XTAL frequency.
#define ADPS2	2
#define ADPS1	1
#define ADPS0	0

//ADC Multiplexer Selection Register
#define ADMUX	(*((volatile u8 *)(0x27)))
//These two bits for setting reference voltage.
/* s1 s0
*  0 0 AREF, Internal Vref turned off
*  0 1 AVCC with external capacitor at AREF pin
*  1 0 Reserved
*  1 1 Internal 2.56V Voltage Reference with external capacitor at AREF pin
*/
#define REFS1	7
#define REFS0	6
//Write one to ADLAR to left adjust the result.
#define ADLAR	5
//Input Channel and Gain Selections
#define MUX4	4
#define MUX3	3
#define MUX2	2
#define MUX1	1
#define MUX0	0

#define SFIOR	*((volatile u8*)0x50)

//macro to unmasking global interrupt before save context
#define ISR_NOBLOCK		__attribute__ ((interrupt))
//macro to tell compile don't add any extra assembly codes (don't add context).
#define ISR_NAKED		__attribute__ ((naked))

//macro like func to init an isr function.
#define ISR(vector,...)   void vector(void) __attribute__ ((signal)); __VA_ARGS__ \
void vector(void)
#define ADC_VECT				__vector_16




#define ADC_MASK		0x01
#define ADC_REG_MASK	0b11111111
#define ADC_TRUE		(1)
#define ADC_FALSE		(0)




#endif