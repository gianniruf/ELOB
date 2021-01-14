/*
 * adc.c
 *
 * @Author:		Gianni Ruf
 * Version	Datum			Name			Change
 * 1.0		10. Sep 2019	Gianni Ruf		Creation
 * 1.1		19. Dec 2020	Gianni Ruf		Update
 *
 * Description
 *	Library for the ADC on the ELOB.
 *	Already done:
 *		- Potentiometer
 *		- snsTemp
 *
 * Hardware
 *	ELOB (Elektroniker Board, ZLV-Mint)
 *	Controller: ATMega2560 (16 MHz)
 *
 * Requirements from Main:
 *	- Interrupts
 */ 

//***INCLUDES***
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../includes/elob_adc.h"

//***DEFINES***
#define muxVRef (ADMUX |= (VRef_sel<<6))	// VRef_sel 0b00...0b11



/**
* Interrupt ADC-Conversion-Completes (ADIF)
* Read, Start
*/
ISR(ADC_vect)
{
	adc_read(&potentiometer);
	adc_start();
}


/**
* Init
* @param: VRef_sel 0...3
* @param: muxChannel_sel 0x00...0x1F
* @param: divFakt_clk 0b000...111
*/
void adc_init(unsigned char VRef_sel, adc_t *myADC, unsigned char divFakt_clk)
{	
	ADCSRA |= (1<<7);		//ADC Enable
	muxVRef;				//
	ADMUX |= myADC->channel & 0x0F;		//MUX4:0 (Modus Vergleich)
	ADCSRB |= myADC->channel & 0x10;	//MUX5
	ADCSRA |= divFakt_clk;	//Clockdivision
	
	ADCSRA |= (1<<3);	//ADC-Interrupt enable (ADIE)
}

/**
* Start
*/
void adc_start(void)
{
	ADCSRA |= (1<<6);	//Start-Bit
}

/**
* Statusabfrage
* @return: status	1=beendet
*/
unsigned char adc_state(void)
{
	return (ADCSRA & (1<<4));	//Start-Bit, 1 = beendet
}

/**
* Read & Convert with min/max
* @param: *myADC adresse von datenstruktur (min/max muss ausgefühlt sein)
*/
void adc_read(adc_t *myADC)
{
	myADC->result_raw = ADC;
	
	//Verhältniss (Realer Messbereich) zu Digitalausgabe * Rohwert(0...1023) + Minimumwert (Achsenabschnitt)
	myADC->result = ((myADC->result_MAX - myADC->result_MIN) / 1024.0) * myADC->result_raw + myADC->result_MIN;
}