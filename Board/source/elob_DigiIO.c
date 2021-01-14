/*
 * elobGPIO.c
 *
 * Created: 18.03.2020 17:38:20
 * Version	Datum			Name			Change
 * 1.0		18. Mar 2019	Gianni Ruf		Creation
 * 1.1		19. Dec 2020	Gianni Ruf		Update
 *
 * Description
 *	Library to use the GPIO on the ELOB.
 *	Already done:
 *		- switches
 *		- LEDs
 *		- Buttons
 *
 * Hardware
 *	ELOB (Elektroniker Board, ZLV-Mint)
 *	Controller: ATMega2560 (16 MHz)
 *
 * Requirements from Main:
 *	-
 */ 

//***INCLUDES***
#include "../includes/elob_DigiIO.h"
#include <avr/io.h>

//***DEFINES***

//***STRUKTUREN***

//***VARIABLEN GLOBAL***

//***FUNKTIONSPROTOTYPEN***

//***INTERRUPTS***

//***FUNKTIONEN***
/**
 * Erkennt flanken
 * @param:	pointer auf Struktur flanke von gew�nschtem PIN
 */
void flankenerkennung_digi(struct flanken_digi *myPIN)
{
	unsigned char state_pin = 0;
	state_pin = *(myPIN->pin);	//speichert aktuellen Zustand (sicher dass keine �nderung w�hrend durchlauf)
	
	(myPIN->flanke_pos) = state_pin & (~(myPIN->old));
	(myPIN->flanke_neg) = (~(state_pin)) & myPIN->old;
	(myPIN->old) = state_pin;
}

/**
 * l�sst LEDs blinken
 * @param:	pointer auf Struktur blinken mit gew�nschtr/em LED
 */
void ledBlinken(struct blinken *myLED)
{
	if(myLED->frequenz_pv > myLED->frequenz_sp)
		myLED->frequenz_pv = 0;
	if(myLED->frequenz_pv < (myLED->frequenz_sp / myLED->dutycycle))
		LEDs |= myLED->led_position;
	else
		LEDs &= ~myLED->led_position;
	myLED->frequenz_pv++;
}

/**
 * initialisiert Digitals I/O
 */
void elobDigiIO_init()
{
	DDRA = 0xFF;	//Output
	DDRC = DDRJ = 0x00;	//Input
}