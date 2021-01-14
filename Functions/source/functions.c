/*
 * functions.c
 *
 * Created: 03.05.2020 10:14:53
 * Author: rufg
 */ 

//***INCLUDES***
#include <avr/io.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"

//***DEFINES***

//***STRUKTUREN***

//***VARIABLEN GLOBAL***

//***FUNKTIONSPROTOTYPEN***

//***FUNKTIONEN***

/**
 * Wandelt Grad in Radiant um
 * @param:	winkel in °
 * @return:	winkel in PI
 */
float degToRad(float winkel)
{
	float radiant;	//Winkel in Radiant
	radiant = winkel * M_PI / 180;
	return radiant;
}

/**
 * Wandelt Radiant in Winkel um
 * @param:	winkel in PI	(float)
 * @return:	winkel in °		(float)
 */
float radToDeg(float winkel)
{
	float degree;	//winkel in grad
	degree = (winkel * 180) / M_PI;
	return degree;
}

/**
 * Generiert Zufallszahl zwischen MIN und MAX
 * @param:	MIN	minimum	(int)
 * @param:	MAX	maximum	(int)
 * @return:	zahl zufallszahl	(int)
 */
unsigned int randomnumber(unsigned int MIN, unsigned int MAX)
{
	static unsigned char ersterDurchgang = 1;	//um srand aufzurufen
	
	if(ersterDurchgang)
	{
		srand((unsigned int) time(0));
		ersterDurchgang = 0;
	}

	unsigned int bereich = MAX - MIN + 1;
	return MIN + (bereich * rand() / (RAND_MAX + 1));
}
