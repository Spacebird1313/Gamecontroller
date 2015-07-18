#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "encoder.h"

volatile char EncoderPosition;			//Volatile globale variabelen: dit voorkomt dat de compiler de code verkeerd optimaliseerd. De compiler merkt dat de interrupt methodes niet worden bereikt tijdens codeuitvoering
										//en zal de variabele standaard op 0 zetten en houden. Het woord 'volatile' zal de compiler dwingen de code niet te optimaliseren voor die variabelen. Als de interrupt wordt uitgevoerd
										//zal de waarde van de variabelen worden gewijzigd.

void EncoderInit(void)
{
	PORTC.DIRCLR = 0b00110000;			//Set Input
	PORTC.PIN4CTRL = 0b00011000;		//5-3: output conf: pull up, 2-0: input conf: both edges
	PORTC.PIN5CTRL = 0b00011000;
	PORTC.INTCTRL = 0b00000101;			//3-2: interrupt 1: low level interrupt, 1-0: interrupt 0: low level interrupt
	PORTC.INT0MASK = 0b00010000;		//interrupt 0: encoder poort 4
	//PORTC.INT1MASK = 0b00100000;		//interrupt 1: encoder poort 5 - verdubbeling resolutie
	PMIC.CTRL = 0b10000001;				//7: Round-robin Scheduling enabled, 1: Low-level interrupt enabled
}

int EncoderGetPos(void)
{
	return EncoderPosition;
}

ISR(PORTC_INT0_vect)
{
	_delay_ms(5);										//wacht 5ms voor uitlezing flank (debounce tijd)
	
	PORTC.INTFLAGS = 0b00000011;						//Reset interruptflag
	
	if((PORTC.IN & 0x10) == 0x10)						//PC4 rising edge
	{
		if((PORTC.IN & 0x20) == 0x20)					//PC5 hoog
		{
			EncoderPosition = EncoderPosition - 1;		//Encoder decrementeren
		}
		else											//PC5 laag
		{
			EncoderPosition = EncoderPosition + 1;		//Encoder incrementeren
		}
	}
	else												//PC4 falling edge
	{
		if((PORTC.IN & 0x20) == 0x20)					//PC5 hoog
		{
			EncoderPosition = EncoderPosition + 1;		//Encoder incrementeren
		}
		else											//PC5 laag
		{
			EncoderPosition = EncoderPosition - 1;		//Encoder decrementeren
		}
	}
}

ISR(PORTC_INT1_vect)
{
	_delay_ms(5);										//wacht 5ms voor uitlezing flank (debounce tijd)
	
	PORTC.INTFLAGS = 0b00000011;						//Reset interruptflag
	
	if((PORTC.IN & 0x20) == 0x20)						//PC5 rising edge
	{
		if((PORTC.IN & 0x10) == 0x10)					//PC4 hoog
		{
			EncoderPosition = EncoderPosition + 1;		//Encoder incrementeren
		}
		else											//PC4 laag
		{
			EncoderPosition = EncoderPosition - 1;		//Encoder decrementeren
		}
	}
	else												//PC5 falling edge
	{
		if((PORTC.IN & 0x10) == 0x10)					//PC4 hoog
		{
			EncoderPosition = EncoderPosition - 1;		//Encoder decrementeren
		}
		else											//PC4 laag
		{
			EncoderPosition = EncoderPosition + 1;		//Encoder incrementeren
		}
	}
}

