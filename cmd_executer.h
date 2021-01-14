/*
 * cmd_executer.h
 *
 * Created: 29.08.2020 09:54:30
 * Author: rufg
 *
 * VERSION	AUTHOR			DATE			CHANGES
 * 1.0		Gianni Ruf		29. Aug 2020	create Library
 * 1.1		Gianni Ruf		10. Sep 2020	Rename (Parser --> cmd_executer)
 *
 * FUNCTIONS
 * find Tokens
 */ 


#ifndef PARSER_H_
#define PARSER_H_

//***DEFINITIONS***
#define MAX_TOKENS	8

#define NOTFOUND	-1
#define ILLEGALPTR	-2

//***VARIABLES GLOBAL***
unsigned int* tokensBeginn[MAX_TOKENS];

//***commands***
char Cmd_set[]   = "set";
char Cmd_get[]   = "get";
char Cmd_reset[] = "reset";

//***tabel witch pointers on cmd***
char * CmdSet[] = {
	Cmd_set,
	Cmd_get,
	Cmd_reset,
	0
};

//***FUNCTIONSPROTOTYPING***

/**
 * @brief:	find beginnings of Tokens, Return Pointers to it
 * @param:	string
 * @return:	void (tokensBeginn[] will be filled)
 * (@return:	tokensBeginn[] will be filled)
 */
unsigned char tokensFind(unsigned char string[]);

/**
 * Check for Commands and execute them
 * @param:	void (is using the 'tokenBeginn[]')
 * @return:	0 = no Error, 1 = Command not Found
 * *List of Commands*
 *	led:	Value of 2end Token on PORTA
 */
unsigned char cmd_executer();

/**
 * @brief:	Reset memory
 * @param:	void
 * @return:	void
 */
void parserReset(void);

/**
 * Find a Command
 * @param:	pVarName	Pointer on the Command (Token)
 * @param:	table		Pointer on the Table witch the Commands
 * @return:	ILLEGALPTR, NOTFOUND, index of table with the matched cmd
 */
char lookUpString(const char* pVarName, const char* table[]);



#endif /* PARSER_H_ */