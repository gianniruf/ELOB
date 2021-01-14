/*
 * I2C.c
 *
 * Created: 09.10.2019 10:43:01
 * Author: rufg
 *
 * VERSION	AUTOR		DATUM		AENDERUNGEN
 * 1.0		Gianni Ruf	09-OKT-2019	Erstellung
 * 1.1		Gianni Ruf	18-SEP-2020	Anpassung Kommentare, Verschiebung von I2CState_* DEFINES
 */ 

//***INCLUDES***
#include "I2C.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//***DEFINES***
#define I2C_StartTransmission	(TWCR = 0b11000101)

#define I2CIntFlag_read				(TWCR & (1<<7))
#define I2CIntFlag_clear			(TWCR |= (1<<7))	//The TWINT Flag must be cleared by software by writing a logic one to it.
#define I2CState_DoneSTART			(0x08)			//Statusregister (TWSR)
#define I2CState_DoneSlvAdressW		(0x18)
#define I2CState_DoneSlvAdressR		(0x40)
#define I2CState_DoneData			(0x28)
#define I2CState_DoneREPSTART		(0x10)
#define I2CState_DoneRecDataACK		(0x50)
#define I2CState_DoneRecDataNACK	(0x58)
#define I2C_ACK_en					(TWCR |= (1<<6))
#define I2C_ACK_dis					(TWCR &= ~(1<<6))

//***VARIABLEN***
unsigned char readAktiv = 0;	//1 = lesen aktiv
unsigned char writeAktiv = 0;	//1 = schreiben aktiv

unsigned char state_read;
unsigned char state_write;
unsigned char state_position;

unsigned char slvAdress;
unsigned char wordAdress;
unsigned char anzBytes;

//***FUNKTIONSPROTOTYPEN***

/**
 * Write Data to Slave
 */
void I2C_write(void);

/**
 * Read Data from Slave
 */
void I2C_read(void);
void I2C_STOPcondition(void);

/**
 * Sendet START condition
 */
void I2C_STARTcondition(void);

/**
 * Adressierung des Slave
 * @param: Adresse 7-Bit
 * @param:	Read/Write (TIPP: use the DEFINE)
 */
void I2C_SlvAdress(unsigned char, unsigned char );

/**
 * Adressiert word in Slave
 * @param:	8-Bit-Adresse des Wordes im Slave
 */
void I2C_WordAdress(unsigned char);

/**
 * Gibt letztes Byte zurück
 * @return:		daten: 8-Bit Daten letztes recive
 */
unsigned char I2C_dataRead(void);

/**
 * Ruft WordAdress auf (schreibt Daten)
 * @param; Daten 8-Bit
 */
void I2C_dataWrite(unsigned char);	//Daten

/**
 * reflect the status of the TWI logic and the 2-wire Serial Bus
 * @return state:	HEX: 08, 10, 20, 28, 30, 38
 */
unsigned char I2C_readState(void);

/**
 * Adressiert word in Slave
 */
void I2C_positon(void);



ISR(TWI_vect)
{
	if(readAktiv)
		I2C_read();
	if(writeAktiv)
		I2C_write();
}



/**
 * Initialisiert TWI
 * @param:	twbr:	0...255 für TWBR- TWI Bit Rate Register
 * @param:	twps:	Prescaler Value 1, 4, 16, 64
 */
void I2C_init(unsigned char twbr																																														, unsigned char twps)
{
	TWBR = twbr;	//BitRate
	TWSR = ((twps == 4 || twps == 64)? 1 : 0);	//Prescaler-Bits
	TWSR = (((twps == 16 || twps == 64) ? 1 : 0) <<1);	//
	TWAMR = 254;	//Adress Mask Register (alle Ok)
	TWCR |= (1<<6);	//ACK-Enable
}

/**
 * Sendet START condition
 */
void I2C_STARTcondition(void)
{
	TWCR = 0b11101101;	//TWINT, TWSTA, TWEN = 1 MUSS; TWEA, TWWC = 1 (ACK Enable)
}

/**
 * Übernimmt Parameter, ruft Funktion zum ersten mal auf
 * @param slvAdress:	Adresse des Bausteins, 7-Bit
 * @param wordAdress:	Adresse des Bytes im Slave, 8-Bit
 * @param anzBytes:		Anzahl Bytes am Stück
 */
void I2C_write_start(unsigned char slvA, unsigned char wordA, unsigned char anzByt)	//SlvAdress, WordAdress, AnzBytes (amStück)
{
	slvAdress = slvA;
	wordAdress = wordA;
	anzBytes = anzByt;
	state_write = 0;
	I2C_write();
}

/**
 * Übernimmt Parameter, ruft Funktion zum ersten mal auf
 * @param slvAdress:	Adresse des Bausteins, 7-Bit
 * @param wordAdress:	Adresse des Bytes im Slave, 8-Bit
 * @param anzBytes:		Anzahl Bytes am Stück
 */
void I2C_read_start(unsigned char slvA, unsigned char wordA, unsigned char anzByt)		//SlvAdress, WordAdress, AnzBytes (amStück)
{
	slvAdress = slvA;
	wordAdress = wordA;
	anzBytes = anzByt;
	state_read = 0;
	I2C_read();
}


/**
 * Write Data to Slave
 */
void I2C_write(void)
{
	static unsigned int indexWrite;
	
	switch(state_write)
	{
		case 0:
			indexWrite = 0;
			state_write = 1;
			writeAktiv = 1;
			I2C_STARTcondition();
			break;
		case 1:
			state_write = 2;
			I2C_SlvAdress(slvAdress, I2C_W);
			break;
		case 2:
			state_write = 3;
			I2C_WordAdress(wordAdress);
			break;
		case 3:
			if (indexWrite < anzBytes)
			{
				I2C_dataWrite(i2c_dataWrite[indexWrite]);
			} 
			else
			{
				state_write = 4;
			}
			indexWrite++;
			break;
		case 4:
			state_write = 0;
			writeAktiv = 0;
			i2c_writeFinished = 1;
			writeAktiv = 0;
			I2C_STOPcondition();
			break;
		default:
			state_write = 0;
			writeAktiv = 0;
			writeAktiv = 0;
			break;
	}
}

/**
 * Read Data from Slave
 */
void I2C_read(void)
{
	static unsigned int indexRead;
	
	switch(state_read)
	{
		case 0:
			indexRead = 0;
			readAktiv = 1;
			I2C_positon();
			//I2C_positon();
			break;
		case 1:
			state_read = 2;
			I2C_STARTcondition();
			break;
		case 2:
			state_read = 4;
			I2C_SlvAdress(slvAdress, I2C_R);
			break;
		case 4:
			if (indexRead < (anzBytes - 1))
			{
				I2CIntFlag_clear;
				state_read = 5;
			} 
			else if(!anzBytes)
			{
				state_read = 7;
			}
			else
			{
				I2C_ACK_dis;
				I2CIntFlag_clear;
				state_read = 6;
			}
			break;
		case 5:
			state_read = 4;
			i2c_dataRead[indexRead] = I2C_dataRead();
			indexRead++;
			break;
		case 6:
			state_read = 7;
			i2c_dataRead[indexRead] = I2C_dataRead();
			break;
		case 7:
			state_read = 0;
			readAktiv = 0;
			i2c_readFinished = 1;
			readAktiv = 0;
			I2C_STOPcondition();
			break;
		default:
			state_read = 0;
			readAktiv = 0;
			break;
	}
}

/**
 * Adressiert word in Slave
 */
void I2C_positon(void)
{
	switch(state_position)
	{
		case 0:
			state_position = 1;
			I2C_STARTcondition();
			break;
		case 1:
			state_position = 2;
			I2C_SlvAdress(slvAdress, I2C_W);
			break;
		case 2:
			state_read = 1;
			state_write = 3;
			state_position = 0;
			I2C_WordAdress(wordAdress);
			break;
	}
}



/**
 * Sendet STOP condition
 */
void I2C_STOPcondition(void)
{
	TWCR = 0b11011101;
}


/**
 * reflect the status of the TWI logic and the 2-wire Serial Bus
 * @return state:	HEX: 08, 10, 20, 28, 30, 38
 */
unsigned char I2C_readState(void)
{
	unsigned char prescaler_old = (TWSR & (0b11));
	TWSR &= ~(0b11);	//Prescaler = 0-->kein einfluss
	unsigned char state = TWSR;
	TWSR |= prescaler_old;
	return state;
}

/**
 * Gibt letztes Byte zurück
 * @return:		daten: 8-Bit Daten letztes recive
 */
unsigned char I2C_dataRead(void)
{
	unsigned char daten = TWDR;
	return daten;
}

/**
 * Adressierung des Slave
 * @param: Adresse 7-Bit
 * @param:	Read/Write (TIPP: use the DEFINE)
 */
void I2C_SlvAdress(unsigned char adress, unsigned char rw)
{
	TWDR = (adress << 1);
	TWDR |= rw;
	I2C_StartTransmission;
}

/**
 * Adressiert Pointer in Slave
 * @parma	wordAdress	Adressiert Register in Slave
 */
void I2C_WordAdress(unsigned char wordAdress)
{
	TWDR = wordAdress;
	I2C_StartTransmission;
}

/**
 * Ruft WordAdress auf (schreibt Daten)
 * @param; Daten 8-Bit
 */
void I2C_dataWrite(unsigned char data)
{
	I2C_WordAdress(data);
}
