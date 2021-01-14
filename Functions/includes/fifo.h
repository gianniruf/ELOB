/*
 * fifo.h
 *
 * Created: 14.05.2020 08:33:05
 *  Author: rufg
 */ 


#ifndef FIFO_H_
#define FIFO_H_

//***DEFINES***

//hier kommen die Codes rein

//***digiTaster***
//0x0X
#define FIFO_DIGITASTER_PRESS		0x01
#define FIFO_DIGITASTER_REALISE		0x02

//***tastenmatrix***
//0x1X
#define FIFO_TASTENMATRIX_PRESS		0x11
#define FIFO_TASTENMATRIX_REALISE	0x12

//***i2c***
//0x2X
#define FIFO_I2C_READDATA			0x21	//Mastermode, read from Slave
#define FIFO_I2C_WROTEDATA			0x22	//Mastermode, write to Slave
#define FIFO_I2C_GOTADDRESSED		0x23	//Slavemode, got Addressed
#define FIFO_I2C_SENDDATA			0x24	//Slavemode, sand data to Master
#define FIFO_I2C_GOTDATA			0x25	//Slavemode, got Data from Master
#define FIFO_I2C_ERROR				0x26	//Error, see I2C.error

//***uart***
//0x3X
#define FIFO_UART_GOTDATA			0x31
#define FIFO_UART_ERROR				0x32

//***cmd_executer***
//0x4X
#define FIFO_CMDEXE_ERROR			0x41
#define FIFO_CMDEXE_INV_PARAM		0x42
#define FIFO_CMDEXE_CMD_NOTFOUND	0x43

//***VARIABLEN***

//***FUNKTIONSPROTOTYPEN***

/**
 * initialisiert den Speicher FIFO
 */
void fifo_init(void);

/**
 * schreibt den Event in den Ringbuffer
 * @param:	event		eventcode (nicht 0!!!)	1Byte
 * @return:	overflow	1=overflow				1Byte
 */
unsigned char fifo_write(unsigned char);

/**
 * liest den Eventcode aus dem Ringbuffer
 * @return: eventcode	eventcode (0=no code)	1Byte
 */
unsigned char fifo_read(void);

#endif /* FIFO_H_ */