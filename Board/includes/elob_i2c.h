/*
 * !!! NOT WORKING YET !!!!
 */

/*
 * elob_i2c.h
 *
 * Created: 17.12.2020 08:44:35
 * Version	Datum			Name			Change
 * 1.0		17. Dec 2019	Gianni Ruf		Creation
 * 2.0		19. Dec 2020	Gianni Ruf		Update
 *
 * Description
 *	Library for the ADC on the ELOB.
 *	Already done:
 *		- send
 *
 * Hardware
 *	ELOB (Elektroniker Board, ZLV-Mint)
 *	Controller: ATMega2560 (16 MHz)
 *
 * Requirements from Main:
 *	- Interrupts
 *	- optional FIFO (define USE_FIFO)
 */ 


#ifndef ELOB_I2C_H_
#define ELOB_I2C_H_

//***INCLUDES***
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

//***DEFINES***
enum I2C_states{I2C_IDLE = 0, I2C_STARTING, I2C_STARTED, I2C_RESTARTED, I2C_SLV_ADDRESSED, I2C_SLV_REG_ADDRESSED, I2C_WRITE, I2C_READ, I2C_STOPPING, I2C_GOT_ADDRESSED};
enum I2C_error{I2C_NO_ERR = 0, I2C_INVALID_FUNCTION_PARAM, I2C_GOT_NACK, I2C_BUSY, I2C_MISSING_MEMBER, I2C_UNDEF_ERROR};
enum I2C_modes{I2C_NO,	I2C_MASTER_R,		I2C_MASTER_W,		I2C_SLAVE_R,		I2C_SLAVE_W};
//						isMaster, Read		isMaster,write		isSlave, send		isSlave, get
#define I2C_SIZE_DATABUFFER	10

//***i2c-addresses***
#define I2C_ADDRESS_RTC	0b1101000

//***VARIABLEN GLOBAL***
typedef struct{
	uint8_t data_slave[I2C_SIZE_DATABUFFER];		//Data Buffer SlaveMode, for Read AND write
	uint8_t data_master[I2C_SIZE_DATABUFFER];	//Data Buffer MasterMode, for Read AND write
	uint8_t state;		//state of I2C protocoll
	uint8_t mode_uart;		//Master/slave, Read/write
	uint8_t error;
} elobI2C_t;

elobI2C_t I2C;

//***FUNKTIONSPROTOTYPEN***

/**
 * Initialisiert I2C on the ELOB
 * @param:	bitRate 0..255
 * @param:	clkPrescaler 1, 4, 16, 64
 * @param:	slvModeAddress	(0x01..0x7F)
 * @return:	error
 *
 * Standartmode is Slave-Mode
 */
uint8_t elobI2C_init(uint8_t bitRate, uint8_t clkPrescaler, uint8_t slvModeAddress);

/**
 * Start a writing of Data to Slave
 * @param:		slvAdress		Adress of Slave (7-Bit, lower-Bits)
 * @param:		slvRegAdress	RegNumber of the first Byte (8-Bit)
 * @param:		nBytes			number of Bytes to read (1..I2C_SIZE_DATABUFFER)
 * Databuffer must be filled befor the function-call!!!
 *
 * At End the FIFO will get a message if USE_FIFO is defined
 *
 * if an ERROR happens, it stops the communication, and set an Value to I2C.error .
 * The FIFO will get a message if USE_FIFO is defined
 * It does NOT reset/restart/retry automatically
 */
void elobI2C_masterMod_writeData_start(uint8_t slvAdress, uint8_t slvRegAdress, uint8_t nBytes);

/**
 * Start reading Data from Slave
 * @param:		slvAdress		Address of Slave (7-Bit, lower-Bits)
 * @param:		slvRegAdress	RegNumber of the first Byte (8-Bit)
 * @param:		nBytes			number of Bytes to read (1..I2C_SIZE_DATABUFFER)
 * Data buffer will be filled
 *
 * At End the FIFO will be get a message if USE_FIFO is defined
 *
 * if an ERROR happens, it stops the communication, and set an Value to I2C.error .
 * The FIFO will get a message if USE_FIFO is defined
 * It does NOT reset/restart/retry automatically
 */
void elobI2C_masterMod_readData_start(uint8_t slvAdress, uint8_t slvRegAdress, uint8_t nBytes);

/**
 * Reset the I2C
 * @return: error	(0=no error, else: error)
 */
uint8_t elobI2C_reset(void);

#endif /* ELOB_I2C_H_ */