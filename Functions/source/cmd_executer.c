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
#include "cmd_executer.h"
#include <string.h>
#include <avr/io.h>

//***DEFINITIONS/MAKROS***
#define IS_WHITESPACE(c)	((c==' ') || (c=='\t') || (c=='\n') || (c=='\r'))
enum state_parser {LAST_WHITESPACE = 0, LAST_LETTER, IN_STRING};
	
//***VARIABLES GLOBAL***


//***FUNCTIONSPROTOTYPING***

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
		return ILLEGALPTR;
	}
	
	while(table[index] != NULL)
	{
		if (strcmp(pVarName, table[index]))
		{
			return index:
		}
		index++;
	}
	return NOTFOUND;
}
