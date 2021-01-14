/*
 * elob7Seg.c
 *
 * Created: 18.03.2020 09:59:04
 *  Author: rufg
 */ 

//***INCLUDES***
#include "elob7Seg.h"
#include <avr/io.h>
#include <stdlib.h>

//***DEFINES***
#define en7seg_off	(PORTG &= ~(1<<2))
#define en7seg_on	(PORTG |= (1<<2))
#define datMuliplexer_set	(PORTG = (stelle<<0))
#define komma_on	(PORTH |= (1<<7))
#define komma_off	(PORTH &= ~(1<<7))

#define ledSeg_off	(PORTH = 0b00000000)
#define ledSeg0	(PORTH = 0b00111111)
#define ledSeg1	(PORTH = 0b00000110)
#define ledSeg2	(PORTH = 0b01011011)
#define ledSeg3	(PORTH = 0b01001111)
#define ledSeg4	(PORTH = 0b01100110)
#define ledSeg5	(PORTH = 0b01101101)
#define ledSeg6	(PORTH = 0b01111101)
#define ledSeg7	(PORTH = 0b00000111)
#define ledSeg8	(PORTH = 0b01111111)
#define ledSeg9	(PORTH = 0b01101111)
#define ledSegA	(PORTH = 0b01110111)
#define ledSegB	(PORTH = 0b01111100)
#define ledSegC	(PORTH = 0b00111001)
#define ledSegD	(PORTH = 0b01011110)
#define ledSegE	(PORTH = 0b01111001)
#define ledSegF	(PORTH = 0b01110001)
#define ledSegMinus		(PORTH = 0b01000000)
#define ledSegDOP_on	(PORTG |= (1<<4))
#define ledSegDOP_off	(PORTG &= ~(1<<4))
#define ledSegHKom_on	(PORTG |= (1<<3))
#define ledSegHKom_off	(PORTG &= ~(1<<3))

//***STRUKTUREN***

//***VARIABLEN GLOBAL***
unsigned char stelle;

//***FUNKTIONSPROTOTYPEN***
unsigned char zifferextraktion(unsigned int zahl, unsigned char position);
void segVorbereitung(unsigned char hochkomma);
void ausgabe(unsigned char);

//***INTERRUPTS***

//***FUNKTIONEN***

/**
 * Steuert die 7Seg an
 */
void ausgabe(unsigned char ziffer)
{
	en7seg_off;
	datMuliplexer_set;
	
	switch(ziffer)
	{
		case 0:
			ledSeg0;
			break;
		case 1:
			ledSeg1;
			break;
		case 2:
			ledSeg2;
			break;
		case 3:
			ledSeg3;
			break;
		case 4:
			ledSeg4;	
			break;
		case 5:
			ledSeg5;
			break;
		case 6:
			ledSeg6;
			break;
		case 7:
			ledSeg7;
			break;
		case 8:
			ledSeg8;
			break;
		case 9:
			ledSeg9;
			break;
		case 10:
			ledSegA;
			break;
		case 11:
			ledSegB;
			break;
		case 12:
			ledSegC;
			break;
		case  13:
			ledSegD;
			break;
		case 14:
			ledSegE;
			break;
		case 15:
			ledSegF;
			break;
		default:
			ledSegMinus;
			break;
	}
}

/**
 * Bereitet die 7Seg vor
 * @param	hochkomma	1=hochkomma aktiv
 */
void segVorbereitung(unsigned char hochkomma)
{
	//***Init***
	DDRG = 0xFF;	//Output
	DDRH = 0xFF;
	
	stelle++;
	if (stelle >= 4)
	{
		if(hochkomma)
			stelle = 1;
		else
			stelle = 0;
	}
}

/**
 * Wandelt Hex-Werte in Dez ausgabewerte um
 * @param: elob7seg_ausbageHex_werte muss mit Dez 0..16 werten abgefüllt sein!!!
 */
void elob7seg_ausgabeHex()
{
	segVorbereitung(0);
	unsigned char ziffer = elob7seg_ausgabeHex_werte[stelle];
	ausgabe(ziffer);
	en7seg_on;	//muss hier eingeschaltet werden
}

/**
* Gibt ZAHLEN auf der 4Fach 7Segmentanzeige aus
* @param: ziffer 0...9
* @param: hochkomma 0,1
* @param: doppelpunkt 0,1
*/
void elob7seg_ausgabeDez(int zahl, unsigned char hKom, unsigned char DoppP, unsigned char komma)
{		
	//NEG ZAHL
	if(zahl < 0)
	{
		if(zahl < -999)
		zahl /= 10;
		komma -= 1;
	}
	//hochkomma (grad)
	if(hKom)
		zahl *= 10;
	
	segVorbereitung(hKom);
	
	unsigned char ziffer;
	if ((zahl > 0) || (stelle < 3))
	{
		ziffer = zifferextraktion(abs(zahl), stelle);
	}
	
	ausgabe(ziffer);
	
	if ((zahl < 0) && (stelle == 3))
	{
		ledSegMinus;
	}
	
	if (hKom)
	{
		ledSegHKom_on;
	}
	else
	{
		ledSegHKom_off;
	}
	
	if (DoppP)
	{
		ledSegDOP_on;
	}
	else
	{
		ledSegDOP_off;
	}
	
	if (stelle == komma && stelle > 0)	//komma am schluss ist überflüssig
	{
		komma_on;
	}
	else
	{
		komma_off;
	}
	
	en7seg_on;	
}

/**
* berechnet ziffer
* @param: zahl 0...9999
* @return: ziffer 0...9
*/
unsigned char zifferextraktion(unsigned int zahl, unsigned char position)
{
	unsigned int ziffer;
	
	switch(position)
	{
		case 3:
			ziffer = zahl / 1000;
			break;
		case 2:
			ziffer = zahl / 100;
			break;
		case 1:
			ziffer = zahl / 10;
			break;
		case 0:
			ziffer = zahl / 1;
			break;
		default:
			ziffer = 8;
			break;
		
	}
	return (ziffer % 10);
}
