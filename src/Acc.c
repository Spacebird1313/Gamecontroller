#include <avr/io.h>
#include "acc.h"


void AccInit(void)
{
	PORTC.DIRSET = 0b01000000;		//Sleep poort configureren
	PORTC.PIN6CTRL = 0b00010001;		//5-3: output conf: pull down
	PORTC.OUTSET = 0b01000000;		//Zet accelerometer actief
	
	PORTA.DIRCLR = 0b00000111;		//Data poorten configureren
	PORTA.PIN0CTRL = 0x00;			//6: niet inverteren, 5-3: output conf: totem, 2-0: input conf: both edge			
	PORTA.PIN1CTRL = 0x00;			
	PORTA.PIN2CTRL = 0x00;			
}

unsigned int AccGetXAxisRaw(void)
{
	return AnalogGetCh(0, 4);			//ADC-kanaal PA0: 0, Differential mode: PA4 - referentiespanning (1V)
}
unsigned int AccGetYAxisRaw(void)
{
	return AnalogGetCh(1, 4);			//ADC-kanaal PA1: 1, Differential mode: PA4 - referentiespanning (1V)
}
unsigned int AccGetZAxisRaw(void)
{
	return AnalogGetCh(2, 4);			//ADC-kanaal PA2: 2, Differential mode: PA4 - referentiespanning (1V)
}

//Calibratie voor bord 3
int AccGetXAxis(unsigned int AccRaw)
{
	return ((AccRaw*5)/2 - 2090);		//Schaalfactor + off-set
}

int AccGetYAxis(unsigned int AccRaw)
{
	return (((AccRaw*5)/2) - 2188);		//Schaalfactor + off-set
}

int AccGetZAxis(unsigned int AccRaw)
{
	return (((AccRaw*7)/5) - 1500);		//Schaalfactor + off-set
}
