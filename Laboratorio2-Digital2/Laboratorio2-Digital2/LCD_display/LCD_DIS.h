/*
 * LCD_DIS.h
 *
 * Created: 1/22/2026 11:55:04 AM
 *  Author: Ervin Gomez 
 */ 


#ifndef LCD_DIS_H_
#define LCD_DIS_H_
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define E (1<<PORTD3)
////////////////////Funciones 
//Función para inicializar el LCD en 8 bits
void bits8INIT(void); //
//Para colocar en el puerto un valor
void PORT_LCD(char a); //
//Función para enviar un comando
void CMD_LCD(char a);//
//Función para enviar un caracter
void Write_Carac(char b); //
//Función para enviar un cadena
void Write_Cad(char *a); //
//Desplazamiento hacia la derecha
void Derech_LCD(void); 
//Desplazamiento hacia la izquierda
void Izqur_LCD(void); 
//Función para establecer el cursor 
void Cursor_LCD(char c, char f); 

#endif /* LCD_DIS_H_ */