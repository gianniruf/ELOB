/*
 * elob_uart.c
 *
 * Created: 17.12.2020 08:45:01
 *  Author: gruf
 */ 

//***INCLUDES***
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../general/includes/fifo.h"
#include "../includes/elob_uart.h"

//***DEFINES***
//***control***
#define CTR_CLEAR_TXINT			(UCSR1A |= (1<<TXC1))
#define CTR_CLEAR_DATAREGEMPTY	(UCSR1A &= ~(1<<UDRE1))
#define CTR_DATAREG1_EMPTY		(UCSR1A & (1<<UDRE1))
#define CTR_FRAME1_ERR			(UCSR1A & (1<<FE1))
#define CTR_FRAME1_ERR_CLEAR	(UCSR1A &= ~(1<<FE1))
#define CTR_IS_DATA1_OVERRUN	(UCSR1A & (1<<DOR1))
#define CTR_DATA1_OVERRUN_CLEAR	(UCSR1A &= ~(1<<DOR1))
#define CTR_IS_PARIRY1_ERROR	(UCSR1A & (1<<UPE1))
#define CTR_PARITY1_ERROR_CLEAR	(UCSR1A &= ~(UPE1))
#define CTR_SPEED1_NORMAL		(UCSR1A &= ~(1<<U2X1))
#define CTR_SPEED1_DOUBLE		(UCSR1A |= (1<<U2X1))
#define CTR_MULTIPROCESSOR1_ON	(UCSR1A |= (1<<MPCM1))
#define CTR_MULTIPROCESSOR1_OFF	(UCSR1A &= ~(1<<MPCM1))

#define CTR_TX1_INT_EN			(UCSR1B |= (1<<TXCIE1))
#define CTR_TX1_INT_DIS			(UCSR1B &= (1<<TXCIE1))
#define CTR_RX1_INT_EN			(UCSR1B |= (1<<RXCIE1))
#define CTR_RX1_INT_DIS			(UCSR1B &= (1<<RXCIE1))
#define CTR_DATAREGEMTY1_INT_EN		(UCSR1B |= (1<<UDRIE1))
#define CTR_DATAREGEMTY1_INT_DIS	(UCSR1B &= ~(1<<UDRIE1))
#define CTR_RECIVE1_EN			(UCSR1B |= (1<<RXEN1))
#define CTR_RECIVE1_DIS			(UCSR1B &= ~(1<<RXEN1))
#define CTR_TRANSMIT1_EN		(UCSR1B |= (1<<TXEN1))
#define CTR_TRANSMIT1_DIS		(UCSR1B &= ~(1<<TXEN1))
#define CTR_CHARSIZE12_SET		(UCSR1B |= (1<<UCSZ12))
#define CTR_CHARSIZE12_CLEAR	(UCSR1B &= ~(1<<UCSZ12))
#define CTR_GOT9BITS1			(UCSR1B & (1<<RXB81))
#define CTR_SEND9BITS1_SET		(UCSR1B |= (1<<TXB81))
#define CTR_SEND9BITS1_CLEAR	(UCSR1B &= ~(1<<TXB81))

#define CTR_SET_MODE1_0_SET		(UCSR1C |= (1<<UMSEL10))
#define CTR_SET_MODE1_0_CLEAR	(UCSR1C &= ~(1<<UMSEL10))
#define CTR_SET_MODE1_1_SET		(UCSR1C |= (1<<UMSEL11))
#define CTR_SET_MODE1_1_CLEAR	(UCSR1C &= ~(1<<UMSEL11))
#define CTR_PARITY1_0_SET		(UCSR1C |= (1<<UPM10))
#define CTR_PARITY1_0_CLEAR		(UCSR1C &= ~(1<<UPM10))
#define CTR_PARITY1_1_SET		(UCSR1C |= (1<<UPM11))
#define CTR_PARITY1_1_CLEAR		(UCSR1C &= ~(1<<UPM11))
#define CTR_STOPBIT1_1			(UCSR1C &= ~(1<<USBS1))
#define CTR_STOPBIT1_2			(UCSR1C |= (1<<USBS1))
#define CTR_CHARSIZE11_SET		(UCSR1C |= (1<<UCSZ11))
#define CTR_CHARSIZE11_CLEAR	(UCSR1C &= ~(1<<UCSZ11))
#define CTR_CHARSIZE10_SET		(UCSR1C |= (1<<UCSZ10))
#define CTR_CHARSIZE10_CLEAR	(UCSR1C &= ~(1<<UCSZ10))
#define CTR_SYNCMODE1_RISING	(UCSR1C |= (1<<UCPOL1))
#define CTR_SYNCMODE1_FALLING	(UCSR1C &= ~(1<<UCPOL1))

//***VARIABLEN GLOBAL***

struct uart_settings{
	uint8_t nWords_send;
	uint8_t nWords_receive;
	uint8_t indexRead;
	uint8_t indexWrite;
	unsigned char parity_loc:2;
	unsigned char mode_loc:2;
	
};

struct uart_settings uart1_sett;

//***FUNKTIONSPROTOTYPEN***
void elobUART1_setCharSize(uint8_t);
void elobUART1_sendData(void);
void elobUART1_receiveData(void);
void elobUART1_stopSend(void);
void elobUART1_error(uint8_t);



//***FUNKTIONEN***

uint8_t elobUART1_init(uint16_t baudrate, uint8_t parity, uint8_t mode_uart, uint8_t charsize){	
	uint8_t error_local = UART_NO_ERROR;
	uint32_t baudrate_loc = baudrate;
	
	if (baudrate)
	{	
		uint16_t ubrr = (F_CPU / (16 * baudrate_loc * 100)) - 1;
		UBRR1 = ubrr;
	} 
	else
	{
		elobUART1_error(UART_INVALID_PARAM);
		error_local = UART_INVALID_PARAM;
	}

	
	CTR_FRAME1_ERR_CLEAR;
	CTR_DATA1_OVERRUN_CLEAR;
	CTR_SPEED1_NORMAL;
	CTR_MULTIPROCESSOR1_OFF;
	
	CTR_RX1_INT_EN;
	CTR_RECIVE1_EN;
	CTR_SEND9BITS1_CLEAR;
	
	if (mode_uart > 3)
	{
		elobUART1_error(UART_INVALID_PARAM);
		error_local = UART_INVALID_PARAM;
	} 
	else
	{
		uart1_sett.mode_loc = mode_uart;
		(mode_uart >= 1) ? CTR_SET_MODE1_0_SET : CTR_SET_MODE1_0_CLEAR;		//0 = async, 1 = sync
		(mode_uart >= 2) ? CTR_SET_MODE1_1_SET : CTR_SET_MODE1_1_CLEAR;		//2 = reserved, 3 = SPI-Master
	}
	
	if (parity > 3)
	{
		elobUART1_error(UART_INVALID_PARAM);
		error_local = UART_INVALID_PARAM;
	} 
	else
	{
		uart1_sett.parity_loc = parity;
		(parity >= 1) ? CTR_PARITY1_0_SET : CTR_PARITY1_0_CLEAR;		//0 = off, 1 = reserved
		(parity >= 2) ? CTR_PARITY1_1_SET : CTR_PARITY1_1_CLEAR;		//2 = even, 3 = odd
	}
	
	if ((charsize >= 5) && (charsize <= 9))
	{
		elobUART1_setCharSize(charsize);
	} 
	else
	{
		elobUART1_error(UART_INVALID_PARAM);
		error_local = UART_INVALID_PARAM;
	}
	
	elobUART1_clearRxMemory();
	
	return error_local;
}

void elobUART_sendData(void){
	uart1.state = UART_SENDING;
	
	CTR_DATAREGEMTY1_INT_DIS;
	CTR_CLEAR_TXINT;
	
	for (uart1_sett.indexWrite = 0; uart1_sett.indexWrite < uart1_sett.nWords_send; uart1_sett.indexWrite++)
	{
		while (!CTR_DATAREG1_EMPTY);
		UDR1 = uart1.dataSend[uart1_sett.indexWrite];
	}
	CTR_TX1_INT_EN;
}

void elobUART1_send(uint8_t length){
	if(uart1.mode_uart == UART_IDLE)
		uart1.mode_uart = UART_WRITE;
	if(uart1.mode_uart == UART_READ)
		uart1.mode_uart = UART_WRITE_READ;
	uart1.state = UART_STARTING;
	CTR_TRANSMIT1_EN;
	CTR_CLEAR_TXINT;
	CTR_CLEAR_DATAREGEMPTY;
	
	uart1_sett.nWords_send = length;
	uart1_sett.indexWrite = 0;
	
	elobUART_sendData();
}

void elobUART_receiveData(void) {
	uart1.dataRead[uart1_sett.indexRead] = UDR1;
	uart1_sett.indexRead++;
	
	if(uart1.dataRead[uart1_sett.indexRead - 1] == UART_ENDSTRING_CHAR)
	{
		uart1.dataRead[uart1_sett.indexRead - 1] = 0;	//delete the CR
		fifo_write(FIFO_UART_GOTDATA);
		uart1.words_received = uart1_sett.indexRead - 1;
	}
}

void elobUART1_stopSend(void){
	uart1.state = UART_STOPPING;
	
	for (int i = 0; i <= uart1_sett.nWords_send; i++)
	{
		uart1.dataSend[i] = 0;
	}
	uart1_sett.nWords_send = 0;
	uart1_sett.indexWrite = 0;
	
	uart1.state = UART_IDLE1;
	if(uart1.mode_uart == UART_WRITE)
		uart1.mode_uart = UART_IDLE;
	if(uart1.mode_uart == UART_WRITE_READ)
		uart1.mode_uart = UART_READ;	
}

void elobUART1_setCharSize(uint8_t value){
	uint8_t use_value;
	if ((value >= 5) && (value <= 8))
		use_value = value - 5;	//5-Bit means 0b000
	if(value == 9)
		use_value = 7;
	
	(use_value & 0b100) ? CTR_CHARSIZE12_SET : CTR_CHARSIZE12_CLEAR;
	(use_value & 0b010) ? CTR_CHARSIZE11_SET : CTR_CHARSIZE11_CLEAR;
	(use_value & 0b001) ? CTR_CHARSIZE10_SET : CTR_CHARSIZE10_CLEAR;
}

void elobUART1_clearRxMemory(void){
	uint8_t i;
	
	for (i = 0; i < UART_SIZE_DATABUFFER; i++)
	{
		uart1.dataRead[i] = 0;
	}
	uart1_sett.indexRead = 0;
}

void elobUART1_error(uint8_t err){
	fifo_write(FIFO_UART_ERROR);
	uart1.error = err;
	
	uart1_sett.nWords_send = 0;
	uart1_sett.indexWrite = 0;
	uart1.state = UART_IDLE1;
	if(uart1.mode_uart == UART_WRITE)
		uart1.mode_uart = UART_IDLE;
	if(uart1.mode_uart == UART_WRITE_READ)
		uart1.mode_uart = UART_READ;
}

//***ISR***

/**
 * Called if there is unread data in the receive-Buffer
 */
ISR(USART1_RX_vect){
	elobUART_receiveData();
}

/**
 * Called if there aren't more data in the transmit-Buffer
 */
ISR(USART1_TX_vect){
	elobUART1_stopSend();
}