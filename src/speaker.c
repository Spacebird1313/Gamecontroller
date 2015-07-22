#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "speaker.h"

void SpeakerInit(void)
{
	//Setup IO
	PORTC.DIRSET = 0b00000001;
	PORTC.PIN0CTRL = 0b00010001;		//6: non-invert, 5-3: output conf: pull down, 2-0: input conf: rising edge
	
	//Setup TC for frequency generation
	TCC0.CTRLA = 0b00000000;		//3-0: Prescaler: uit
	TCC0.CTRLB = 0b00010011;		//7-4: Compare or Capture: channel A (pin: PC0), 2-0: Single-slope PWM
	TCC0.CTRLD = 0x00000000;		//7-5: Event Action: uit, 4: Timer delay: uit, 3-0: Timer event: uit
	TCC0.CTRLE = 0b00000000;		//1-0: Normaal
	
	//Setup TC for duration timing
	TCC1.CTRLA = 0b00000000;		//3-0: Prescaler: uit
	TCC1.CTRLB = 0b00000000;		//7-4: Compare or Capture: uit, 2-0: Normal
	TCC1.CTRLD = 0x00000000;		//7-5: Event Action: uit, 4: Timer delay: uit, 3-0: Timer event: uit
	TCC1.CTRLE = 0b00000000;		//1-0: Normaal
	
	TCC1.INTCTRLA = 0b00000001;		//3-2: Error Interrupt: uit, 1-0: Overflow Interrupt-level: low
}

void SpeakerBeep(int Frequency,int Length)
{
	int countF = ((16000000)/(Frequency))-1;
	int countL = ((16000000)/(1024*(1000/Length)))-1;	//(1/Length)*1000  Conversie ms naar Hz
	TCC0.PER = countF;					//Top-waarde (periode)
	TCC0.CCA = countF/2;					//Duty-cycle (pulsbreedte)
	TCC0.CTRLA = 0b00000001;				//3-0: Prescaler: clk (aan)
	TCC1.PER = countL;					//Top-waarde
	TCC1.CTRLA = 0b00000111;				//3-0: Prescaler: clk/1024 (aan)
	
	//Lengte bepaald zonder interrupt
	//_delay_ms(500);
	//TCC0.CTRLA = 0b00000000;				//3-0: Prescaler: uit
}

ISR(TCC1_OVF_vect)
{
	TCC0.CTRLA = 0b00000000;				//3-0: Prescaler: uit
	TCC0.CTRLFSET = 0b00001000;				//3-2: Restart
	TCC1.CTRLA = 0b00000000;				//3-0: Prescaler: uit
	TCC1.CTRLFSET = 0b00001000;				//3-2: Restart
	playNextNote();
}
