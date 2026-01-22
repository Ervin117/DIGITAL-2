/*
 * DISPLAY.c
 *
 * Created: 1/22/2026 9:30:52 AM
 *  Author: Ervin Gomez 
 */ 

#include "DISPLAY7SEG.h"
int		DIS7SEG[6] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B};
	
void display_inic(void)
{
	DDRD = 0xFF; 
	PORTD = 0x00; 
}

void display7seg(uint8_t cursor)
{
	PORTD = DIS7SEG[cursor]; 
}