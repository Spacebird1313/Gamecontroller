#include "Clock.h"
#include <avr/io.h>

#pragma GCC push_options
#pragma GCC optimize ("O2") //This function must be optimized to avoid too long time interval betwee CCP write and CLK.CTRL write (4 CLK CYC max)

void ClockInit(void)
{
	OSC.XOSCCTRL=0b11001011; //Clock<=16MHz	
	OSC.CTRL=0b01000; //Enable oscillator	
	while (!(OSC.STATUS&0b1000)); //Wait until XTAL osc is ready
	
	//Select XTAL osc as system CLK source
	CCP=0xd8;
    CLK.CTRL=0b0011;
	
}

#pragma GCC pop_options

