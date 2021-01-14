/*
 * elobRGB.c
 *
 * Created: 18.03.2020 10:00:47
 *  Author: rufg
 */ 

//***INCLUDES***
#include "elobRGB.h"
#include <avr/io.h>

//***DEFINES***

//***STRUKTUREN***

//***VARIABLEN GLOBAL***

//***FUNKTIONSPROTOTYPEN***
void timer1_init(void);

//***INTERRUPTS***

//***FUNKTIONEN***

/**
 * Initialisiert RGB, Timer1
 */
void elobRGB_init(void)
{
	timer1_init();
	DDRB = 0xFF;	//RGB; 1 = Output
}

/**
 * setzt Rot-Wert
 * @param:	RGB-Wert Rot 0..255
 */
void elobRGB_setRed(unsigned char rot)
{
	OCR1A = rot;	//Compare-Register Rot
}

/**
 * setzt Grün-Wert
 * @param:	RGB-Wert Grün 0..255
 */
void elobRGB_setGreen(unsigned char gruen)
{
	OCR1B = gruen;	//Compare-Register Grün
}

/**
 * setzt blau-Wert
 * @param:	RGB-Wert blau 0..255
 */
void elobRGB_setBlue(unsigned char blau)
{
	OCR1C = blau;	//Compare-Register Blau
}

/**
 * Setzt Farbwerte
 * @param:	rot		0..255, ^= PWM-Wert (oder vorgefertigtes DEFINE)
 * @param:	gruen	0..255, ^= PWM-Wert
 * @param:	blau	0..255, ^= PWM-Wert
 */
void elobRGB_setColor(unsigned char rot, unsigned char gruen, unsigned char blau)
{
	elobRGB_setRed(rot);
	elobRGB_setGreen(gruen);
	elobRGB_setBlue(blau);
}

/**
 * Resetet Timer1, RGB
 */
void elobRGB_clear(void)
{
	TCCR1A = 0b00000000;
	TCCR1B = 0b00000000;
	OCR1A = 0;		//Compare-Register Rot
	OCR1B = 0;		//Compare-Register Gruen
	OCR1C = 0;		//Compare-Register Blau
}

/**
 * Timer 1 Initialisierung
 */
void timer1_init(void)
{
	TCCR1A = 0b10101001;	//8:2 clear @compare match, set @BOTTOM, (non-inverting mode); 1:0 WGM
	TCCR1B |= (0b01 << 3);	//WGM4:3
	TCCR1B |= 0b100;		//Clock Select, 0b100 = clk/256
	OCR1A = 0;		//Compare-Register Rot
	OCR1B = 0;		//Compare-Register Gruen
	OCR1C = 0;		//Compare-Register Blau
}
