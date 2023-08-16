#include "EEPROM_interface.h"
#include "EEPROM_Private.h"

void EEPROM_Send(u16 address,u8 data)
{
	while(1 == GET_BIT(EECR,EEWE)); //wait till other send operation finish.
	EEAR = address;
	EEDR = data;
	//must close interrupt.
	cli();
	SET_BIT(EECR, EEMWE);
	SET_BIT(EECR,EEWE);
	sei();
}
u8	EEPROM_Receive(u16 address,u8 *data)
{
	u8 error=0;
	EEAR = address;
	if(NULL_PTR == data)
	{
		error =1;
	}
	else if( (address < 0) || (address > 1024))
	{
		error =2;
	}
	else
	{
		while(1 == GET_BIT(EECR,EEWE));
		SET_BIT(EECR,EERE);
		*data = EEDR;	
	}
	
	return (error);
}