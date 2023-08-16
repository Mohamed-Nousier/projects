/*
 * UART.h
 *
 * Created: 2/7/2021 12:00:24 PM
 *  Author: 20115
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#define F_CPU 16000000UL


#define BaudRate 9600
#define DoupleSpeed 0 // we can change it as we like

#if (DoupleSpeed == 0)
	#define MyUBRR ((F_CPU/(16UL*BaudRate))-1) //we should put casting       // from the law in the data sheet UBBR
#else
	#define MyUBRR ((F_CPU/(8UL*BaudRate))-1)
#endif


void Uart_Init();
void Uart_Txchar(char data);
char Uart_Rxchar();
void Uart_Txstring( signed char * string);



#endif /* UART_H_ */