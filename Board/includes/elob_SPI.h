/*
 * elob_SPI.h
 *
 * Created: 28.01.2021 08:59:26
 * Author: gruf
 *
 *	V	DATE			AUTHOR			CHANGE
 *	1.0	28. Jan 2021	Gianni Ruf		Creation
 */ 


#ifndef ELOB_SPI_H_
#define ELOB_SPI_H_

//***DEFINES***
#define ELOB_SPI_BUFFER_SIZE		16

//***VARIABLES***
char elobSPI_dataSend[ELOB_SPI_BUFFER_SIZE];


/**
 * Initialise the SPI-Interface of the ELOB
 * @param: clkSel	4, 16, 64, 128, 2, 8, 32, 64
 */
void elobSPI_init(uint8_t clkSel);

/**
 * Send Data with SPI
 * @param: nBytes		Number of Bytes to send
 * @param: nWordSize	Number of Bytes per Word
 */
void elobSPI_send(uint8_t nBytes, uint8_t nWordSize);

/**
 * Get Data with SPI
 */
void elboSPI_get(void);


#endif /* ELOB_SPI_H_ */