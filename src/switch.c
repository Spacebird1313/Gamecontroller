#include "switch.h"
#include <avr/io.h>

void SwitchInit(void)
{
	PORTD.DIRCLR = 0b11110001;			//Set Input
	PORTD.PIN0CTRL = 0b01011001;		//6: invert, 5-3: output conf: pull up, 2-0: input conf: rising edge
	PORTD.PIN4CTRL = 0b01011001;
	PORTD.PIN5CTRL = 0b01011001;
	PORTD.PIN6CTRL = 0b01011001;
	PORTD.PIN7CTRL = 0b01011001;
}


char SwitchGet(void)
{
	char switchInput = (PORTD.IN & 0xF0) >> 3;	//Schuif B7-B4 naar B4-B1
	
	return switchInput | (PORTD.IN & 0x01);		//Voeg B0 toe aan switchInput
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                