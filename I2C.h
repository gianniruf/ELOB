/*
 * I2C.h
 *
 * Created: 09.10.2019 10:43:21
 * Author: rufg
 * 
 * Library zum Bedienen der ELOB-I2C-Schnittstelle
 * Interrupts müssen im main() freigegeben werden (sei();)
 */ 


#ifndef I2C_H_
#define I2C_H_
//***DEFINES***

#define I2C_R	1	//I2C-Lesen (bei Adresse)
#define I2C_W	0	//I2C-Schreiben (bei Adresse)

//***ARRAYS***
unsigned char i2c_dataWrite[20];	//Daten (20x 8-Bit) welche gesendet werden
unsigned char i2c_dataRead[20];		//Daten (20x 8-Bit) welche erhalten wurden

//***VARIABLEN***
unsigned char i2c_writeFinished;	//1 = schreiben beendet
unsigned char i2c_readFinished;	//1 = lesen beendet

//***FUNKTIONSPROTOTYPEN***

/**
 * Initialisiert I2C-Schnittstelle
 * @param:	TWBR	0...255 für TWBR- TWI Bit Rate Register
 * @pram:	TWPS	Prescaler Value 1, 4, 16, 64
 * @return:	void
 */
void I2C_init(unsigned char, unsigned char);	

/**
 * Startet eine I2C-SCHREIB-Kommunikation
 * @param:	SlvAdress	7-Bit
 * @param:	WordAdress	8-Bit
 * @param:	AnzBytes die am Stück geschrieben werden	0..20
 * @return:	void
 */
void I2C_write_start(unsigned char, unsigned char, unsigned char);

/**
 * Startet eine I2C-LESE-Kommunikation
 * @param:	SlvAdress	7-Bit
 * @param:	WordAdress	8-Bit
 * @param:	AnzBytes die am Stück gelesen werden	0..20
 * @return:	void
 */
void I2C_read_start(unsigned char, unsigned char, unsigned char);



#endif /* I2C_H_ */