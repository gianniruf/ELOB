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
//0000XXXXX
#define FIFO_DIGITASTER_PRESS		0b00000001
#define FIFO_DIGITASTER_REALISE		0b00000010

//***tastenmatrix***
//0001XXXX
#define FIFO_TASTENMATRIX_PRESS		0b00010001
#define FIFO_TASTENMATRIX_REALISE	0b00010010

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