/*
 * UART.c
 *
 * Created: 2/7/2021 12:00:13 PM
 *  Author: 20115
 */ 

#include "UART.h"



void Uart_Init() // we will use polling not interrupt
{
	// 8-bit, No PAR , 1-bit STOP
	UCSRB |= ((1<<RXEN)|(1<<TXEN));
	UCSRC |= ((1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0)); // the frame is 8-bits & 
	UBRRL = (char)MyUBRR; //EX: 0x0ACC;  UBRRL = 0xCC;
	UBRRH = (char)(MyUBRR>>8); // 0x0ACC>>8 : 0x00A ::::: UBRRH = 0x0A;
}


void Uart_Txchar(char data)
{
	/*i will not be able to send anything until i make sure that (UDRE in UCSRA is empty)*/
	while(!((UCSRA>>UDRE)&1));  // READ_BIT law , while( ! TRUE) = get out
	//while(((UCSRA>>UDRE)&1) == 0);
	UDR = data; // send char
	
}


char Uart_Rxchar()
{
	char Ret=0;
	if(1 == ((UCSRA>>RXC)&1))
	{
		Ret = UDR;
	}
	else
	{
		Ret = '0';
	}
	return Ret; // return data(that has been sent)
}


void Uart_Txstring( signed char	* string)
{
	/*like the LCD SendString function*/
	unsigned char i=0;
	while(string[i] != '\0')
	{
		Uart_Txchar(string[i]);
		i++;
	}
	
}