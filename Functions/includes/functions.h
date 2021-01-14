/*
 * functions.h
 *
 * Created: 03.05.2020 10:14:41
 *  Author: rufg
 *
 * Hier sind einige nützliche Funktionen
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

//***INCLUDES***

//***DEFINES***

//***STRUKTUREN***

//***VARIABLEN GLOBAL***

//***FUNKTIONSPROTOTYPEN***

/**
 * Wandelt Grad in Radiant um
 * @param:	winkel in °		(float)
 * @return:	winkel in PI	(float)
 */
float degToRad(float winkel);

/**
 * Wandelt Radiant in Winkel um
 * @param:	winkel in PI	(float)
 * @return:	winkel in °		(float)
 */
float radToDeg(float winkel);

/**
 * Generiert Zufallszahl zwischen MIN und MAX
 * @param:	MIN	minimum	(int)
 * @param:	MAX	maximum	(int)
 * @return:	zahl zufallszahl	(int)
 */
unsigned int randomnumber(unsigned int min, unsigned int max);

#endif /* FUNCTIONS_H_ */