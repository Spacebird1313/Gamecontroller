#include "Analog.h"
#include <avr/io.h>

void AnalogInit(void)
{
	ADCA.REFCTRL = 0b0000010;							//6-4: referentie 1V (000), 1: Bandgap enabled (1), 0: temperatuursensor uit (0)
	ADCA.CTRLB = 0b00010000;							//6-5: Current limiet uit (00), 4: signed waardes (1), 3: autorun uit (0), 2-1: uitkomst resolutie: 12 bits rechts (00)
	ADCA.PRESCALER = 0b00000010;						//2-0: Prescaler (Divider 16) : 16Mhz -> 1Mhz
}

int AnalogGetCh(int ChannelPos, int ChannelNeg)
{
	ChannelNeg = ChannelNeg - 4;	//PA4 = 0, PA5 = 1,...
	
	ADCA_CH0_MUXCTRL = ((ChannelPos & 0x0F) << 3) | (ChannelNeg & 0x07);	//Selecteer de samplepoorten	
	
	if(ChannelNeg == -1)
	{
		ADCA_CH0_CTRL = 0b10000001;	//Start sampling in Single ended mode - 7: start conversie, 4-2: Gain (1x), 1-0: single ended mode
	}
	else
	{
		ADCA_CH0_CTRL = 0b10000011;	//Start sampling in Differential ended mode with gain - 7: start conversie, 4-2: Gain (1x), 1-0: differential ended mode met gain
	}
	
	ADCA.INTFLAGS = 0b00000001;		//Reset interrupt flag
	
	ADCA.CTRLA = 0b00000101;		//2: Start ADC, 0: activeer ADC
	
	while(ADCA.INTFLAGS == (0x00)); //Wacht voor voltooien huidige conversie
	
	return ADCA.CH0RES;
}