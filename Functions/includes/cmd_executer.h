/*
 * cmd_executer.h
 *
 * Created: 29.08.2020 09:54:30
 * Author: rufg
 *
 * VERSION	AUTHOR			DATE			CHANGES
 * 1.0		Gianni Ruf		29. Aug 2020	create Library
 * 1.1		Gianni Ruf		10. Sep 2020	Rename (Parser --> cmd_executer)
 * 2.0		Gianni Ruf		26. Dez 2020	New Structure
 *
 * Requirements
 *		fifo.h
 */ 


#ifndef PARSER_H_
#define PARSER_H_

//***DEFINITIONS***
#define MAX_TOKENS	8

//***cmdNumbers***
#define CMDEXE_SET		2
#define CMDEXE_GET		3
#define CMDEXE_PRINT	4
#define CMDEXE_RESET	5

//****hw-number***
#define CMDEXE_HELP		0
#define CMDEXE_HELP1	1
#define CMDEXE_UC		2
#define CMDEXE_RGB		3
#define CMDEXE_RTC		4
#define CMDEXE_LCD		5
#define CMDEXE_7SEG		6
#define CMDEXE_POTI		7
#define CMDEXE_TMP		8

#define N_CMD			8

//***enums***

//***VARIABLES GLOBAL***

//***commands***
//**function**
const char Cmd_set[]	= "set";
const char Cmd_get[]	= "get";
const char Cmd_print[]	= "print";
const char Cmd_reset[]	= "reset";
//**HW-unit**
const char Cmd_help[]	= "?";
const char Cmd_help1[]	= "help";
const char uC[]		= "uC";
const char rgb[]	= "RGB";
const char rtc[]	= "RTC";
const char lcd[]	= "LCD";
const char seg7[]	= "7seg";
const char poti[]	= "poti";
const char tmp[]	= "tmp";

//***tabel witch pointers on cmd***
char * CmdSet[] = {
	Cmd_set,
	Cmd_get,
	Cmd_reset,
	0
};

char * hwSet[] = {
	Cmd_help,
	Cmd_help1,
	uC,
	rgb,
	rtc,
	lcd,
	seg7,
	poti,
	tmp,
	0
};

//***FUNCTIONSPROTOTYPING***

/**
 * @brief:	Reset memory
 * @param:	void
 * @return:	void
 */
void parserReset(void);

/**
 * intepret and execute a command in ASCII-Format
 * @param:	pointer on the text
 * @retrun:	void
 */
void execute_TXTcmd(char * cmd);

#endif /* PARSER_H_ */