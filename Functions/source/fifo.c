/*
 * fifo.c
 *
 * Created: 14.05.2020 08:32:43
 *  Author: rufg
 */ 

//***INCLUDES***
#include <avr/io.h>
#include "../includes/fifo.h"

//***DEFINES***
#define GROESSE_RINGBUFFER	8

//***VARIABLEN***
unsigned char ringbuffer[GROESSE_RINGBUFFER];

//***FUNKTIONEN***

/**
 * initialisiert/Resetet den Speicher FIFO
 */
void fifo_init(void)
{
	for (int i = 0; i<GROESSE_RINGBUFFER; i++)
	{
		ringbuffer[i] = 0;	//alles mit 0 füllen
	}
}

/**
 * schreibt den Event in den Ringbuffer
 * @param:	event		eventcode	1Byte
 * @return:	overflow	1=overflow	1Byte
 */
unsigned char fifo_write(unsigned char eventCode)
{
	//Variablen
	unsigned char overflow;	//1 = overflow
	static unsigned char indexWrite = 0;
	
	//Check for Overflow
	if(ringbuffer[indexWrite] == 0)
		overflow = 0;
	else
		overflow = 1;
	
	//Write Code in buffer
	if(!overflow)	//schreiben möglich
	{
		ringbuffer[indexWrite] = eventCode;
		indexWrite++;
		if(indexWrite >= GROESSE_RINGBUFFER)
			indexWrite = 0;	//zurück auf anfangsposition
	}
	
	return overflow;
}

/**
 * liest den Eventcode aus dem Ringbuffer
 * @return: eventcode	eventcode (0=no code)	1Byte
 */
unsigned char fifo_read(void)
{
	//Variablen
	unsigned char eventCode;
	static unsigned char indexRead = 0;
	
	//Code lesen und löschen
	eventCode = ringbuffer[indexRead];
	ringbuffer[indexRead] = 0;
	
	if(eventCode)	//ein Code vorhanden (nicht 0)
	{	
		indexRead++;
		if(indexRead >= GROESSE_RINGBUFFER)
			indexRead = 0;
	}
	
	return eventCode;
}