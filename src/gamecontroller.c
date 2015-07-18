#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "Clock.h"
#include "USART.h"
#include "Analog.h"
#include "acc.h"
#include "LED.h"
#include "switch.h"
#include "encoder.h"
#include "speaker.h"

struct note {
			 int Frequency;
			 int Length;
			 struct note *Next;
			};
		
unsigned char a,b;
unsigned char ledOn;
signed int RawAccX, RawAccY, RawAccZ;
struct note *note1;
struct note *note2;
struct note *note3;
struct note *note4;
struct note *note5;
struct note *note6;
struct note *note7;
struct note *note8;
struct note *note9;
struct note *note10;
struct note *notePointer;
struct note *lastNote;

void SimpleFunction(void);	//A simple function: transmit digits 0 to 9 to the terminal device
void LoopLicht(void);
void AccLezen(void);
void Beeps(void);
void setNotes(void);
void playNextNote(void);

int main(void)
{	
	//Code geoptimaliseerd voor bordje 3
	//###1###
	 
	//Initialize subsystems
	ClockInit();			//Initialize system clock (16 MHz)
	USARTInit();			//Initialize USART and bind to stdout,stdin
	AnalogInit();			//Initialize ADC
	AccInit();				//Initialize accelerometer system
	LEDInit();				//Initialize LEDs
	SwitchInit();			//Initialize switches
	EncoderInit();			//Initialize encoder
	SpeakerInit();			//Initialize speaker system
	
	//Enable interrupts
	PMIC.CTRL|=0b00000111;  //Enable low, medium, high priority interrupts
	SREG|=0b10000000;		//Globale interrupt enable

	//###2###	
	
	//Print the digits 0 to 9 5x on terminal device
	//Reason this section didn't work: de variabelen 'a' is globaal gedefinieerd. In het subprogramma 'SimpleFunction()' wordt
	//de variabelen 'a' gebruikt. Na dit subprogramma is de waarde van 'a' gelijk aan 10 en zal de lus (a<5) onderbroken worden.
	 
	for (a=0;a<5;a++)
	{
		SimpleFunction();
		printf ("\r\n");
	}	

	//###3###

	//Main program loop
	a=0;
	ledOn=0b00001000;
	
	//Beeps();		//Genereer 3 beeps van 500hz, 1000hz, 1500hz (500 ms)
	setNotes();		//Stel een reeks van noten in
	playNextNote();	//Speel de eerste noot uit de reeks
	
    while(1)
    {
		LoopLicht();																									//LoopLicht sequentie			
		AccLezen();																										//Accelerometer lezen
		printf("$SWITCH %d\r\n", SwitchGet());																			//Switchwaarde over USARTD: 1: center, 2: rechts, 4: beneden, 8: links, 16: boven
		printf("$ACCRAW %d %d %d\r\n", RawAccX, RawAccY, RawAccZ);														//Ongecalibreerde waardes accelerometer x, y, z over USARTD
		printf("$ACC__ %d %d %d\r\n", AccGetXAxis(RawAccX), AccGetYAxis(RawAccY), AccGetZAxis(RawAccZ));				//Gecalibreerde waardes accelerometer x, y, z over USARTD
		printf("$ENC__ %d\r\n", EncoderGetPos());
		//printf ("Counter:%d\r\n",a);
		//printf("%c", 0x55);																							//Stuur de waarde 0x55 uit (0b01010101)
		//a++;
		_delay_ms(20);
    }
}

void SimpleFunction(void)
{
	for (b=0;b<10;b++)
		printf ("%d ",b);
}

void LoopLicht(void)
{
	if(ledOn == 0x08)							//4de bit actief (led: D4)
	{
		ledOn = 0x01;							//1ste bit actief (led: D1)
	}
	else
	{
		ledOn = ledOn << 1; 					//Actieve bit links shiften
	}
	
	LEDSet(ledOn);
}

void AccLezen(void)
{
	RawAccX = AccGetXAxisRaw();					//Lees waardes van ADC - Accelerometer
	RawAccY = AccGetYAxisRaw();
	RawAccZ = AccGetZAxisRaw();
}

void Beeps(void)
{
	SpeakerBeep(1000, 500);
	_delay_ms(500);
	SpeakerBeep(2000, 500);
	_delay_ms(500);
	SpeakerBeep(3000, 500);
}

void setNotes(void)
{
	note1 = (struct note *) malloc (sizeof(struct note));		//Maak de noten aan, alloceer geheugen voor deze noten
	note2 = (struct note *) malloc (sizeof(struct note));
	note3 = (struct note *) malloc (sizeof(struct note));
	note4 = (struct note *) malloc (sizeof(struct note));
	note5 = (struct note *) malloc (sizeof(struct note));
	note6 = (struct note *) malloc (sizeof(struct note));
	note7 = (struct note *) malloc (sizeof(struct note));
	note8 = (struct note *) malloc (sizeof(struct note));
	note9 = (struct note *) malloc (sizeof(struct note));
	note10 = (struct note *) malloc (sizeof(struct note));
	
	note1 -> Frequency = 1000;									//Definieer een noot
	note1 -> Length = 100;
	note1 -> Next = note2;
	
	note2 -> Frequency = 2000;
	note2 -> Length = 100;
	note2 -> Next = note3;
	
	note3 -> Frequency = 3000;
	note3 -> Length = 100;
	note3 -> Next = note4;
	
	note4 -> Frequency = 1000;
	note4 -> Length = 100;
	note4 -> Next = note5;
	
	note5 -> Frequency = 0;
	note5 -> Length = 100;
	note5 -> Next = note6;
	
	note6 -> Frequency = 4000;
	note6 -> Length = 100;
	note6 -> Next = note7;
	
	note7 -> Frequency = 5000;
	note7 -> Length = 100;
	note7 -> Next = note8;
	
	note8 -> Frequency = 6000;
	note8 -> Length = 100;
	note8 -> Next = note9;
	
	note9 -> Frequency = 4000;
	note9 -> Length = 100;
	note9 -> Next = note10;
	
	note10 -> Frequency = 0;
	note10 -> Length = 100;
	note10 -> Next = NULL;
	
	notePointer = note1;
}

void playNextNote(void)
{
	if(notePointer != NULL)
	{
		SpeakerBeep(notePointer->Frequency, notePointer->Length);		//Speel een noot
		lastNote = notePointer;
		notePointer = notePointer->Next;								//Wijzig de pointer naar de nieuwe noot
		free(lastNote);													//Verwijder de laatst afgespeelde noot uit het geheugen
	}
	else																//Geen volgende noot beschikbaar
	{
		free(notePointer);												//Verwijder de laatst afgespeelde noot uit het geheugen
		setNotes();														//Stel een reeks van noten in
		//playNextNote();												//Speel de eerste noot uit de reeks
	}
}