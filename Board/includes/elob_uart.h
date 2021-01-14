/*
 * elob_uart.h
 *
 * Version	Datum			Name			Change
 * 1.0		23. Mar 2019	Gianni Ruf		Creation
 *
 * Description
 *	Library for the UART1 on the ELOB.
 *	Already done:
 *
 * Hardware
 *	ELOB (Elektroniker Board, ZLV-Mint)
 *	Controller: ATMega2560 (16 MHz)
 *
 * Requirements from Main:
 *	- Interrupts
 *	- optional FIFO (define USE_FIFO)
 */


#ifndef ELOB_UART_H_
#define ELOB_UART_H_

//***INCLUDES***
#include <avr/io.h>

//***DEFINES***
#define UART_ENDSTRING_CHAR		('\r')	//CR

//***enumes***
enum UART_error{UART_NO_ERROR = 0, UART_INVALID_PARAM, UART_COMM_ERROR};
enum UART_modes{UART_IDLE = 0, UART_WRITE, UART_READ, UART_WRITE_READ};
enum UART_state{UART_IDLE1 = 0, UART_STARTING, UART_STOPPING, UART_SENDING};

#define UART_SIZE_DATABUFFER	30

//***VARIABLEN GLOBAL***
typedef struct{
	char dataRead[UART_SIZE_DATABUFFER];
	char dataSend[UART_SIZE_DATABUFFER];
	uint8_t error;	//errorValues (view enum)
	uint8_t state;	//state (view enum)
	uint8_t mode_uart;	//mode	(view enum)
	uint8_t words_received;
	}elobUART;
	
elobUART uart1;

//***FUNKTIONSPROTOTYPEN***

/**
 * initialisiert UART 1
 * @param:	baudrate(div 100) ==> 96, 576
 * @param:	parity:	0 = off, 1 = even, 2 = odd
 * @param:	mode:	0 = async, 1 = sync, 3 = SPI master	(2 = reserved)
 * @param:	charsize	Size of a character (5, 6, 7, 8)
 * @return:	error
 */
uint8_t elobUART1_init(uint16_t baudrate, uint8_t parity, uint8_t mode_uart, uint8_t charsize);

/**
 * Send data with UART
 * @param:	length		number of Databytes to send
 */
void elobUART1_send(uint8_t length);

/**
 * Reset the receive-memory and reset the pointer
 */
void elobUART1_clearRxMemory(void);

#endif /* ELOB_UART_H_ */