#include "LED.h"
#include <avr/io.h>

void LEDInit(void)
{
	PORTE.DIRSET = 0b00001111;		//Set Output
	PORTE.PIN0CTRL = 0b01010001;	//6: invert, 5-3: output conf: pull down, 2-0: input conf: rising edge
	PORTE.PIN1CTRL = 0b01010001;
	PORTE.PIN2CTRL = 0b01010001;
	PORTE.PIN3CTRL = 0b01010001;
}

void LEDSet(char bitmask)
{
	bitmask	= bitmask & 0x0F;					//and-functie voor afzondering van eerste 4 bits (B0 --> B3)
	
	PORTE.OUT = (PORTE.OUT & 0xF0) | bitmask;	//behoud laatste 4 bits van register (B4 --> B7) en voeg eerste 4 bits van bitmask toe (B0 --> B3)
}