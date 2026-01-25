/*
 * LCD_DIS.c
 *
 * Created: 1/22/2026 11:54:47 AM
 *  Author: Ervin Gomez 
 */ 

#include "LCD_DIS.h"

//Se establecen las funciones

//Función de inicio para el LCD con 8 bits
void bits8INIT(void)
{	
	DDRD |=(1<<DDD2) | (1<<DDD3) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7);
	DDRB |=(1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3) | (1<<DDB4);
		
	PORTB &= ~((1<<DDB0) | (1<<DDB1) | (1<<DDB2) | (1<<DDB3) | (1<<DDB4));
	PORTD &= ~((1<<DDD2) | (1<<DDD3) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7));

	_delay_ms(20);

	CMD_LCD(0x30);
	_delay_ms(5);
	CMD_LCD(0x30);
	_delay_ms(1);
	CMD_LCD(0x30);

	//Los comandos depende de la cantidad de bits que se necesita
	//Al igual que el proceso de inicialización. 
	// Function Set
	CMD_LCD(0x38);

	// Display ON/OFF
	CMD_LCD(0x08);
	CMD_LCD(0x0C);

	// Entry Mode
	CMD_LCD(0x06);

	// Clear Display
	CMD_LCD(0x01);
	_delay_ms(2);
}
//Función para enviar comandos 
void CMD_LCD(char a)
{
	//RS = 1 //RS = 0
	PORTD &= ~(1<<PORTD2); 
	PORT_LCD(a); 
	//E = 1 //E= 0
	PORTD |= (1<<PORTD3); 
	_delay_ms(50); 
	
	PORTD &= ~(1<<PORTD3); 
	_delay_ms(50); 
}

//Función para colocar un valor 

void PORT_LCD(char a)
{
	//En esta función se debe encender cada bit correspondiente 
	//a el puerto. 
	if(a & 1)
	PORTD |= (1<<PORTD5);
	else 
	PORTD &= ~(1<<PORTD5);
	
	if(a & 2)
	PORTD |= (1<<PORTD6);
	else
	PORTD &= ~(1<<PORTD6);
	
	if(a & 4)
	PORTD |= (1<<PORTD7);
	else
	PORTD &= ~(1<<PORTD7);
	
	if(a & 8)
	PORTB |= (1<<PORTB0);
	else
	PORTB &= ~(1<<PORTB0);
	
	if(a & 16)
	PORTB |= (1<<PORTB1);
	else
	PORTB &= ~(1<<PORTB1);
	
	if(a & 32)
	PORTB |= (1<<PORTB2);
	else
	PORTB &= ~(1<<PORTB2);
	
	if(a & 64)
	PORTB |= (1<<PORTB3);
	else
	PORTB &= ~(1<<PORTB3);
	
	if(a & 128)
	PORTB |= (1<<PORTB4);
	else
	PORTB &= ~(1<<PORTB4);
}
//Función para enviar un caracter
void Write_Carac(char b)
{ 
	
	PORTD |= (1<<PORTD2);
	PORT_LCD(b); 
	
	PORTD |= (1<<PORTD3);
	_delay_ms(4); 
	PORTD &= ~(1<<PORTD3);
	
	
}

void Write_Cad(char *a)
{
	int i; 
	for (i = 0; a[i] != '\0'; i++)
	Write_Carac(a[i]); 
}

void Derech_LCD(void)
{
	CMD_LCD(0x01); 
	CMD_LCD(0x0C); 
}

void Izqur_LCD(void) 
{
	CMD_LCD(0x01); 
	CMD_LCD(0x08); 
}

void Cursor_LCD(char c, char f)
{
	char temp; 
	if (f==1)
	{
		temp = 0x80 + c - 1; 
	}
	else if (f == 2)
	{
		temp = 0xC0 + c - 1; 
	}
	CMD_LCD(temp);
}