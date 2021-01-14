/*
 * elob_adc.h
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


#ifndef ADC_H_
#define ADC_H_

//***DEFINES***
#define ADC_CHANNEL_POTI	(ADC3)
#define ADC_CHANNEL_NTC		(ADC2)

//***ENUM***
enum channel_t{ADC0=0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7, ADC8 = 32, ADC9, ADC10, ADC11, ADC12, ADC13, ADC14, ADC15};

//***STRUCKTUREN***
typedef struct 
{
	enum channel_t channel;
	unsigned int result_raw;	//Wandlungsresultat Rohwert
	unsigned int convInterval_pv;	//Wandlungsinterval
	unsigned int convInerval_sp;	//ms
	float result;		//Wandlungsresultat
	float result_old;	//altes Wandlungsresultat
	float result_MIN;	//minimaler Wandlungswert
	float result_MAX;	//maximaler wandlungswert
}adc_t;

adc_t potentiometer;
adc_t temperature;

//***FUNKTIONSPROTOTYPEN***

/**
* Init
* @param: VRef_sel 0...3
* @param: pointer auf Struktur
* @param: divFakt_clk 0b000...111
*/
void adc_init(unsigned char, adc_t *, unsigned char);

void adc_start(void);

/**
* Statusabfrage
* @return: status	1=beendet
*/
unsigned char adc_state();

/**
* Read & Convert with min/max
* @param: *myADC adresse von datenstruktur (min/max muss ausgefühlt sein)
*/
void adc_read(adc_t *);


#endif /* ADC_H_ */