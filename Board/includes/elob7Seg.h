/*
 * elob7Seg.h
 *
 * Created: 18.03.2020 09:58:38
 * Author: rufg
 */ 


#ifndef ELOB7SEG_H_
#define ELOB7SEG_H_

//***DEFINES***

//***STRUKTUREN***

//***VARIABLEN GLOBAL***
unsigned char elob7seg_ausgabeHex_werte[4];

//***FUNKTIONSPROTOTYPEN***

/**
 * Gibt jeweils eine Stelle aus(wechselt Ziffer selbständig)
 * @param	zahl	gewünschte Zahl, -999..9999
 * @param	hKom	Hochkomma 1=anzeigen
 * @param	DoppP	Doppelpunkt 1=anzeigen
 * @param	komma	Stelle des Komma 0..3, wenn 0 wird nicht angezeigt
 */
void elob7seg_ausgabeDez(int zahl, unsigned char hKom, unsigned char DoppP, unsigned char komma);

/**
 * Gibt 4 Hex-Ziffern aus, multiplext selber
 * @param	elob7seg_ausgabeHex_werte muss ausgefüllt sein!!!
 */
void elob7seg_ausgabeHex();

//***INTERRUPTS***

#endif /* ELOB7SEG_H_ */
