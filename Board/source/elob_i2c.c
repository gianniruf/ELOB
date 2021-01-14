/*
 * elob_i2c.c
 *
 * Created: 17.12.2020 08:44:20
 * Version	Datum			Name			Change
 * 1.0		17. Dec 2019	Gianni Ruf		Creation
 * 2.0		19. Dec 2020	Gianni Ruf		Update
 *
 * Description
 *	Library for the ADC on the ELOB.
 *	Already done:
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
#include <stdint.h>
#include <avr/delay.h>
#include "../includes/elob_i2c.h"
#include "../../general/includes/fifo.h"

//***DEFINES***
//***cotrol***
#define RESET_CONTROL_REG	(TWCR = 0x00)
#define SEND_START		((1<<TWSTA)|(1<<TWEN))
#define ENABLE_ACK		(TWCR |= (1<<TWEA))
#define DISABLE_ACK		(TWCR &= ~(1<<TWEA))
#define SET_START		(TWCR |= (1<<TWSTA))
#define RESET_START		(TWCR &= ~(1<<TWSTA))
#define SET_STOP		(TWCR |= (1<<TWSTO))
#define RESET_STOP		(TWCR &= ~(1<<TWSTO))
#define IS_W_COLLITION	(TWCR & (1<<TWWC))
#define ENABLE_I2C		(TWCR |= (1<<TWEN))
#define DISABLE_I2C		(TWCR &= ~(1<<TWEN))
#define INTERRUPT_EN	(TWCR |= (1<<TWIE))
#define INTERRUPT_DIS	(TWCR &= ~(1<<TWIE))
#define CLEAR_INTERRUPT	(TWCR |= (1<<TWINT))	//clear = write logic 1 to it
#define I2C_GETSTATE	(TWSR & 0b11111000)
#define EN_GEN_CALL		(TWAR |= (1<<TWGCE))
#define DIS_GEN_CALL	(TWAR &= ~(1<<TWGCE))

//***usage***
#define READ			1
#define WRITE			0

//***state-register***
#define I2C_STATE_START_DONE		0x08
#define I2C_STATE_REPSTART_DONE		0x10
#define I2C_STATE_SLVADDRW_ACK		0x18	//write Mode got ACK
#define I2C_STATE_SLVADDRW_NACK		0x20	//write Mode got NACK
#define I2C_STATE_DATASEND_ACK		0x28	//write Mode got ACK
#define I2C_STATE_DATASEND_NACK		0x30	//write Mode got NACK
#define I2C_STATE_LOST_ACK			0x38
#define I2C_STATE_SLVADDRR_ACK		0x40	//read Mode got ACK
#define I2C_STATE_SLVADDRR_NACK		0x48	//read Mode got NACK
#define I2C_STATE_DATAACKSEND_DONE	0x50	//read Mode received Data and sand ACK
#define I2C_STATE_DATANACKSEND_DONE	0x58	//read Mode received Data and sand NACK
#define I2C_STATE_GOTADDRESSEDW_ACK	0x60
#define I2C_STATE_GOTADDRESSEDW_NACK	0x68
#define I2C_STATE_GENCALL_ACK		0x70
#define I2C_STATE_GENCALL_NACK		0x78
#define I2C_STATE_GOTDATAADD_ACK		0x80	//SlvGet-mode, Databyte Receved
#define I2C_STATE_GOTDATAADD_NACK		0x88
#define	I2C_STATE_GOTDATAGC_ACK			0x90
#define I2C_STATE_GOTDATAGC_NACK		0x90
#define I2C_STATE_GOTREPSTART_STOP		0xA0
#define I2C_STATE_GOTADDRESSEDR_ACK		0xA8
#define I2C_STATE_GOTADDRESSEDR_NACK	0xB0
#define I2C_STATE_DATASENDED_GOTACK		0xB8
#define I2C_STATE_DATASENDED_GOTNACK	0xC0



//***VARIABLEN GLOBAL***
uint8_t slvAddress_local;	//Address of current Slave (7-Bit)
uint8_t slvRegAdress_local;	//Address of startregister (8-Bit)
uint8_t nData_local;		//Number of Bytes to read/write
uint8_t SLV_MODE_ADDRESS;	//Slave-Mode-Address (7-Bit)

//***FUNKTIONSPROTOTYPEN***
void elobI2C_masterMod_writeData(void);
void elobI2C_masterMod_readData(void);
//void elobI2C_slaveMode_writeData(void);
//void elobI2C_slaveMode_readData(void);
void elobI2C_sendSlvAddress(uint8_t rw);
void elobI2C_sendWriteData(uint8_t data);
void elobI2C_sendStart(void);
void elobI2C_sendStop(void);
void elobI2C_addressSlave(uint8_t read_write);
void elobI2C_setToSlave(void);
void elobI2C_error(uint8_t err);

//***ISR***
/**
 * I2C-ISR
 * Check Mode  (Mst/Slv, R/W) and Call the correct function
 */
ISR(TWI_vect){
	switch(I2C.mode_uart){
		case I2C_MASTER_R:
			elobI2C_masterMod_readData();
			break;
		
		case I2C_MASTER_W:
			elobI2C_masterMod_writeData();
			break;
			
		case I2C_SLAVE_R:
	//		elobI2C_slaveMode_readData();	
			break;
			
		case I2C_SLAVE_W:
	//		elobI2C_slaveMode_writeData();
			break;
		
		default:
			if (I2C_GETSTATE == I2C_STATE_GOTADDRESSEDR_ACK)	//got Addressed in slvMode
			{
				I2C.state = I2C_SLAVE_W;
			}
	}
}

//***FUNKTIONEN***

uint8_t elobI2C_reset(void){
	TWBR = 0;
	TWSR = 0;
	TWAMR = 0;
	TWCR = 0;
	I2C.error = elobI2C_init(100, 16, SLV_MODE_ADDRESS);
	for (int i = 0; i < I2C_SIZE_DATABUFFER; i++)
	{
		I2C.data_master[i] = 0;
		I2C.data_slave[i] = 0;
	}
	
	return I2C_NO_ERR;
}

uint8_t elobI2C_init(uint8_t bitRate, uint8_t clkPrescaler, uint8_t slvModeAddress){
	TWBR = bitRate;	//BitRate
	TWSR = ((clkPrescaler == 4 || clkPrescaler == 64)? 1 : 0);	//Prescaler-Bits
	TWSR = (((clkPrescaler == 16 || clkPrescaler == 64) ? 1 : 0) <<1);	//
	TWAMR = 254;	//Adress Mask Register (alle Ok)
	SLV_MODE_ADDRESS = slvModeAddress;
	INTERRUPT_EN;
	elobI2C_setToSlave();		//Slave Mode is standart, so it can receive messages
	
	return I2C_NO_ERR;
}

void elobI2C_setToSlave(void){
	TWAR &= ~(0xFE);	//clear the 7 higher Bits
	TWAR |= (SLV_MODE_ADDRESS << TWA0);	//only the 7 higher-Bits
	ENABLE_ACK;
	RESET_START;
	RESET_STOP;
	ENABLE_I2C;
}

void elobI2C_masterMod_writeData_start(uint8_t slvAdress, uint8_t slvRegAdress, uint8_t nBytes){
	if (I2C.state == I2C_IDLE)
	{
		I2C.mode_uart = I2C_MASTER_W;
		slvRegAdress_local = slvRegAdress;
		slvAddress_local = slvAdress;
		nData_local = nBytes;
		I2C.state = I2C_STARTING;
		elobI2C_masterMod_writeData();
	}
	else{
		I2C.error = I2C_BUSY;
	}
}

void elobI2C_masterMod_writeData(void){
	static uint8_t nByte_pv = 0;
	
	switch(I2C.state){
		case I2C_STARTING:	//set Startcondition
		case I2C_STARTED:	//set Slv-Address
		case I2C_SLV_ADDRESSED:	//Set RegisterAddress
			elobI2C_addressSlave(WRITE);
			break;
			
		case I2C_WRITE:	//send Databytes
			if (I2C_GETSTATE == I2C_STATE_DATASEND_ACK)	//ACK Data is same like ACK RegAddress
			{
				if (nByte_pv < nData_local)
					elobI2C_sendWriteData(I2C.data_master[nByte_pv]);
				else
					I2C.state = I2C_STOPPING;
				nByte_pv++;
			} 
			else
			{
				if (I2C_GETSTATE == I2C_STATE_DATASEND_NACK)
					elobI2C_error(I2C_GOT_NACK);
				else
					elobI2C_error(I2C_UNDEF_ERROR);
			}

			break;
			
		case I2C_STOPPING:	//send stop
			nByte_pv = 0;
			elobI2C_sendStop();
			#ifdef USE_FIFO		//write to Fifo if its used
				if(I2C.error == I2C_NO_ERR)
					fifo_write(FIFO_I2C_WROTEDATA);
			#endif
			elobI2C_setToSlave();	//standard mode is slave-mode
			I2C.state = I2C_IDLE;
			I2C.mode_uart = I2C_NO;
			break;

		default:
			nByte_pv = 0;	//reset for next Time
			I2C.error = I2C_UNDEF_ERROR;
			break;	
	}
}

void elobI2C_masterMod_readData(void){
	static uint8_t nByte_pv = 0;	//count received bytes
	
	switch(I2C.state){
		case I2C_STARTING:	//set Startcondition
		case I2C_STARTED:	//set Slv-Address
		case I2C_SLV_ADDRESSED:	//Set RegisterAddress
			elobI2C_addressSlave(WRITE);	//for Addressing WRITE is needed, after Restart READ
			break;
			
		case I2C_SLV_REG_ADDRESSED:	//send Restart
			if(I2C_GETSTATE == I2C_STATE_DATASEND_ACK)
			{	
//				elobI2C_sendStop();
				TWDR = (slvAddress_local << 1);
				TWDR |= READ;
				elobI2C_sendStart();
				I2C.state = I2C_RESTARTED;
			}
			else
			{
				elobI2C_error(I2C_UNDEF_ERROR);
			}
			break;
			
		case I2C_RESTARTED:
			if (I2C_GETSTATE == I2C_STATE_START_DONE)
			{
				elobI2C_sendSlvAddress(READ);
				I2C.state = I2C_READ;
			}
			else
			{
				elobI2C_error(I2C_UNDEF_ERROR);
			}
			break;
		
		case I2C_READ:
			if (I2C_GETSTATE == I2C_STATE_SLVADDRR_ACK)
			{
				if (nData_local)
				{	//at least on byte to read
					if (nByte_pv < (nData_local - 1))	//not last byte
					{
						ENABLE_ACK;
						I2C.data_master[nByte_pv] = TWDR;	//Save Data register in Buffer
						CLEAR_INTERRUPT;
					}
					else
					{	//Last Byte
						DISABLE_ACK;
						I2C.data_master[nByte_pv] = TWDR;	//Save Data register in Buffer
						CLEAR_INTERRUPT;
						I2C.state = I2C_STOPPING;
					}
				}
				else
				{
					elobI2C_error(I2C_INVALID_FUNCTION_PARAM);
				}
			} 
			else
			{
				elobI2C_error(I2C_UNDEF_ERROR);	//undef, member should be here
			}
			break;
		
		case I2C_STOPPING:
			nByte_pv = 0;
			elobI2C_sendStop();
			#ifdef USE_FIFO		//write to Fifo if its used
				if(I2C.error == I2C_NO_ERR)		//else the error is already writen in fifo
					fifo_write(FIFO_I2C_READDATA);
			#endif
			elobI2C_setToSlave();	//standard mode is slave-mode
			I2C.state = I2C_IDLE;
			I2C.mode_uart = I2C_NO;
			break;
			
	}
}

void elobI2C_masterMod_readData_start(uint8_t slvAdress, uint8_t slvRegAdress, uint8_t nBytes){
	if (I2C.state == I2C_IDLE)
	{
		I2C.mode_uart = I2C_MASTER_R;
		slvRegAdress_local = slvRegAdress;
		slvAddress_local = slvAdress;
		nData_local = nBytes;
		I2C.state = I2C_STARTING;
		elobI2C_masterMod_readData();
	}
	else{
		I2C.error = I2C_BUSY;
		#ifdef USE_FIFO
			uint8_t overflow = fifo_write(I2C_BUSY);
			overflow++;
		#endif
	}
}

void elobI2C_addressSlave(uint8_t read_write){
	switch(I2C.state)
	{
		case I2C_STARTING:	//set Startcondition
			I2C.state = I2C_STARTED;
			elobI2C_sendStart();
			break;
		
		case I2C_STARTED:	//set Slv-Address
			if (I2C_GETSTATE == I2C_STATE_START_DONE)
			{	//worked, go on
				elobI2C_sendSlvAddress(read_write);
				I2C.state = I2C_SLV_ADDRESSED;
			}
			else
			{	//error, Stop
				elobI2C_error(I2C_UNDEF_ERROR);
			}
			break;
		
		case I2C_SLV_ADDRESSED:	//Set RegisterAddress
			if ((I2C_GETSTATE == I2C_STATE_SLVADDRR_ACK) || (I2C_GETSTATE == I2C_STATE_SLVADDRW_ACK))
			{	//got ACK, go on
				elobI2C_sendWriteData(slvRegAdress_local);
				if(I2C.mode_uart == I2C_MASTER_W)
					I2C.state = I2C_WRITE;
				if(I2C.mode_uart == I2C_MASTER_R)
					I2C.state = I2C_SLV_REG_ADDRESSED;
			} 
			else
			{	//got NACK, Stop and give ERROR
				if ((I2C_GETSTATE == I2C_STATE_SLVADDRR_NACK) || (I2C_GETSTATE == I2C_STATE_SLVADDRW_NACK))
					elobI2C_error(I2C_MISSING_MEMBER);
				else
					elobI2C_error(I2C_UNDEF_ERROR);
			}
			break;
	}
}

void elobI2C_error(uint8_t err){
	I2C.state = I2C_STOPPING;
	I2C.error = err;
	#ifdef USE_FIFO
		uint8_t overflow = fifo_write(FIFO_I2C_ERROR);
		overflow++;
	#endif
}

void elobI2C_sendStop(void){
	SET_STOP;
	CLEAR_INTERRUPT;
}

void elobI2C_sendStart(void){
	TWAR = 0x00;	//remove Slave-Mode-Address
	SET_START;
	RESET_START;
}

void elobI2C_sendWriteData(uint8_t data){
	TWDR = data;
	CLEAR_INTERRUPT;
}

void elobI2C_sendSlvAddress(uint8_t rw){
	TWDR = (slvAddress_local << 1);
	TWDR |= rw;
	CLEAR_INTERRUPT;
}
/*
void elobI2C_slaveMod_readData(void){

}


void elobI2C_slaveMode_writeData(void){

}*/