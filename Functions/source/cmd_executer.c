/*
 * parser.c
 *
 * Created: 29.08.2020 09:54:14
 * Author: rufg
 *
 * VERSION	AUTHOR			DATE			CHANGES
 * 1.0		Gianni Ruf		29. Aug 2020	create Library
 * 1.1		Gianni Ruf		10. Sep 2020	Rename (Parser --> cmd_executer)
 */ 

//***INCLUDES***
#include "../includes/cmd_executer.h"
#include "../includes/fifo.h"
#include "../../ELOB/includes/elob7segV2.h"
#include "../../ELOB/includes/elob_adc.h"
#include "../../ELOB/includes/elob_i2c.h"
#include "../../ELOB/includes/elob_RGB.h"
#include "../../ELOB/includes/elob_uart.h"
#include <string.h>
#include <avr/io.h>

//***DEFINITIONS/MAKROS***
#define IS_WHITESPACE(c)	((c==' ') || (c=='\t') || (c=='\n') || (c=='\r'))
enum state_parser {LAST_WHITESPACE = 0, LAST_LETTER, IN_STRING};
	
//***VARIABLES GLOBAL***
unsigned int* tokensBeginn[MAX_TOKENS];
const char error_notFound[]	= "ERROR cmd not Found\n";
const char help_hwUnits[]	= "There are follows HW-Units:\n";
const char help_swCmd[]		= "There are follows SW-Cmd's:\n";
const char help_usage[]		= "Usage: HW Cmd (Value)\n";

//***FUNCTIONSPROTOTYPING***
char lookUpString(const char* pVarName, const char* table[]);

//***FUNCTIONS***

/**
 * @brief:	Reset memory
 * @param:	void
 * @return:	void
 */
void parserReset(void)
{
	//***variables local***
	unsigned char index;
	
	for (index = 0; index < MAX_TOKENS; index ++)
	{
		tokensBeginn[index] = 0;
	}
}

/**
 * @brief:	find beginnings of Tokens, Return Pointers to it
 * @param:	string
 * @return:	uchar	number of tokens 
 * (@return:	tokensBeginn[] will be filled)
 */
unsigned char tokensFind(unsigned char string[])
{
	//***variables local***
	unsigned char indexString = 0;		//Pointer in String
	unsigned char indexBeginnings = 0;	//Pointer in Memory of Beginnings
	unsigned char symbol = 1;			//actual symbol, beginning != 0
	unsigned char state = LAST_WHITESPACE;	//Status Parser
		
	while(symbol != 0)
	{
		symbol = string[indexString];
		switch(state)
		{
			case LAST_WHITESPACE:
				if (!IS_WHITESPACE(symbol))
				{
					if(symbol == '"')
					{
						state = IN_STRING;
						string[indexString] = 0;
						tokensBeginn[indexBeginnings] = &string[indexString+1];
					}
					else
					{
						tokensBeginn[indexBeginnings] = &string[indexString];
						state = LAST_LETTER;
					}
					indexBeginnings++;					
				}
				break;
			
			case LAST_LETTER:
				if (IS_WHITESPACE(symbol))
				{
					string[indexString] = 0;	//like a end of a string
					state = LAST_WHITESPACE;
				}
				break;
				
			case IN_STRING:
				if (symbol == '"')
				{
					state = LAST_WHITESPACE;
					string[indexString] = 0;	//like a end of a string
				}
				break;
				
			default:
				break;
		}
		indexString++;
	}
	return indexBeginnings;	//=number of Tokens
}

/**
 * Check for Commands and execute them
 * @param:	void (is using the 'tokenBeginn[]')
 * @return:	0 = command found and executed, 1 = Command not Found
 * *List of Commands*
 *	led:	Value of 2end Token on PORTA
 */
unsigned char cmd_executer(void)
{
	unsigned char cmdFound = 1;	//1 = no Command Found
	const unsigned char cmdLed[] = "led";
	
	if (!strcmp((tokensBeginn[0]), cmdLed))//0 = are similar
	{
		PORTA = *(tokensBeginn[1]);	//LEDs = Number
		cmdFound = 0;
	}
	return cmdFound;
}

char lookUpString(const char* pVarName, const char* table[])
{
	unsigned char index = 0;
	
	if (pVarName == NULL || table == NULL)
	{
		return;
	}
	
	while(table[index] != NULL)
	{
		if (strcmp(pVarName, table[index]))
		{
			return index;
		}
		index++;
	}
	return;
}

int8_t search_cmd(const char * table[], const char ** cmd){
	int8_t cmdNumber = -1;
	uint8_t idx = 0;
	for (idx = 0; idx <= sizeof(*table); idx++);
	{
		if (!strcmp(*cmd, *table[idx]))
			cmdNumber = idx;
	}
	return cmdNumber;
}

void execute_TXTcmd(char * cmd){
	if (!cmd)
	{
		fifo_write(FIFO_CMDEXE_INV_PARAM);
	} 
	else
	{
		uint8_t nTokens;
		nTokens = tokensFind(cmd);
		uint8_t cmdNr = search_cmd(hwSet, tokensBeginn[0]);
		
		switch(cmdNr){
			case CMDEXE_UC:
				exe_uC();
				break;
			case CMDEXE_RGB:
				break;
			case CMDEXE_RTC:
				break;
			case CMDEXE_LCD:
				break;
			case CMDEXE_7SEG:
				break;
			case CMDEXE_POTI:
				break;
			case CMDEXE_TMP:
				break;
			default:
				//print List of Commands and Usage
				if (!((cmdNr == CMDEXE_HELP) || (cmdNr == CMDEXE_HELP1)))
				{
					strcpy(uart1.dataSend, error_notFound);
					elobUART1_send(sizeof(error_notFound));
				}
				exe_help();
				break;
		}
	}
}

void exe_uC(void){
	
}

void exe_RGB(void){
	
}

void exe_RTC(void){
	
}

void exe_LCD(void){
	
}

void exe_7seg(void){
	
}

void exe_poti(void){
	
}

void exe_tmp(void){
	
}

void exe_help(void){
	//Send HW-Units
	strcpy(uart1.dataSend, help_hwUnits);
	elobUART1_send(sizeof(help_hwUnits));
	for (uint8_t i; i <= sizeof(hwSet); i++)
	{
		strcpy(uart1.dataSend, *hwSet[i]);
		elobUART1_send(sizeof(hwSet[i]));
	}
}