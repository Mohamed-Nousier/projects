#ifndef EEPROM_PRIVATE_H_
#define EEPROM_PRIVATE_H_

#define sei()	__asm__ __volatile__ ("sei" ::)
#define cli()	__asm__ __volatile__ ("cli" ::)

/* EEPROM Control Register */
#define EECR	(*(volatile unsigned char*)0x3C)

/* EEPROM Data Register */
#define EEDR	(*(volatile unsigned char*)0x3D)

/* EEPROM Address Register */
#define EEAR	(*(volatile unsigned short*)0x3E)
#define EEARL	(*(volatile unsigned char*)0x3E)
#define EEARH	(*(volatile unsigned char*)0x3F)

/*EEPROM*/
#define EERE	0
#define EEWE	1
#define EEMWE	2
#define EERIE	3

#endif /* EEPROM_PRIVATE_H_ */