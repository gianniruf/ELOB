/*
 * elob_SPI.c
 *
 * Created: 28.01.2021 08:59:13
 *  Author: gruf
 */ 

//***INCLUDES***
#include <stdint.h>
#include <avr/io.h>
#include <string.h>
#include "elob_SPI.h"

//***DEFINE***
//***registers***
#define CTR_INT_EN		(SPCR |= (1<<SPIE))
#define CTR_INT_DIS		(SPCR &= ~(1<<SPIE))
#define CTR_SPI_EN		(SPCR |= (1<<SPE))
#define CTR_SPI_DIS		(SPCR &= (1<<SPE))
#define CTR_LSB_1ST		(SPCR |= (1<<DORD))
#define CTR_MSB_1ST		(SPCR &= ~(1<<DORD))
#define CTR_MODE_MSTR	(SPCR |= (1<<MSTR))
#define CTR_MODE_SLV	(SPCR &= ~(1<<MSTR))
#define CTR_CLOCK_FAL	(SPCR |= (1<<CPOL))
#define CTR_CLOCK_RIS	(SPCR &= ~(1<<CPOL))
#define CTR_CLK_SEL1_ON		(SPCR |= (1<<SPR1))
#define CTR_CLK_SEL1_OFF	(SPCR &= ~(1<<SPR1))
#define CTR_CLK_SEL0_ON		(SPCR |= (1<<SPR0))
#define CTR_CLK_SEL0_OFF	(SPCR &= ~(1<<SPR0))

#define CTR_CS_INACTIV	(PORTB |= (1<<0))
#define CTR_CS_ACTIV	(PORTB &= ~(1<<1))

//***VARIABLES GLOBAL***

//***FUNCTIONS***

void elobSPI_init(uint8_t clkSel){
	SPCR = 0;
	CTR_INT_EN;
	CTR_SPI_EN;
	CTR_MSB_1ST;
	CTR_MODE_MSTR;
	CTR_CLOCK_RIS;
	if((clkSel == 32) || (clkSel == 64) ||(clkSel == 128))
		CTR_CLK_SEL1_ON;
	else
		CTR_CLK_SEL1_OFF;
	if ((clkSel == 8) || (clkSel == 16) ||(clkSel == 64) || (clkSel == 128))
		CTR_CLK_SEL0_ON;
	else
		CTR_CLK_SEL0_OFF;
	
}


void elobSPI_send(uint8_t nBytes, uint8_t nWordSize){
	uint8_t nBytes_sent = 0;
	
	for (nBytes_sent = 0; nBytes_sent < nBytes;)
	{	
		CTR_CS_ACTIV;
		do
		{
			strcpy(&SPDR, elobSPI_dataSend[nBytes_sent]);	//copy the Data to Dataregister
			nBytes_sent++;
		}while (!(nBytes_sent % nBytes));
		CTR_CS_INACTIV;
	}	
}


void elboSPI_get(void){
	
}