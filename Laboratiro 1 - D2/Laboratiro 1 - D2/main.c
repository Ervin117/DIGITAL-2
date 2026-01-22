/*
 * Laboratorio 1 - D2.c
 *
 * Created: 1/15/2026 7:08:27 PM
 * Author : Ervin Gomez 231226
 */ 

/****************************************/
// Encabezado
#include <avr/io.h>
#include <avr/interrupt.h>
#include "7SEG/DISPLAY7SEG.h"

//int		DIS7SEG[6] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B};
uint8_t contador = 0;
uint8_t cont = 5; 
uint8_t banTimer = 0; 
uint8_t decdJ1 = 0; 
uint8_t decdJ2 = 0; 
uint8_t J1 = 0;
uint8_t gana = 0;

/****************************************/
// Function prototypes
void PinChange();
void TMR0();

/****************************************/
// Main Function
int main(void) {
	// Configura PORTD como salida para el 7SEG
	//DDRD = 0xFF;  
	//PORTD = DIS7SEG[0]; 
	display_inic(); 
	
	//Configuración de leds del primer jugador 
	DDRB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB5);
	PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB5)); 
	
	//Configuración de leds del segundo jugador 
	DDRC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2);
	PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2)); 

	//Configuración de las entradas (Botones) 
	DDRC &= ~((1 << PORTC3) | (1 << PORTC4) | (1<<PORTC5));  
	PORTC |= (1 << PORTC3) | (1 << PORTC4)|(1<<PORTC5);  //Activacion de pull-ups
	
	UCSR0B = 0x00;
	
	PinChange();
	TMR0();
	display7seg(0); 
	
	sei();
	while (1)
	{
		
	}
}
/****************************************/
// NON-Interrupt subroutines
void PinChange(void) {
	//Configuración del Pin_Change
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13);
}

void TMR0(void) ///////Tengo dudas 
{
	//Configuración del Clock del reloj
	//CLKPR = (1<<CLKPCE);
	//CLKPR = (1<<CLKPS3);  // 1MHz
	TCCR0A = 0x00; 
	TCCR0B |= (1<<CS02);
	TCNT0 = 131;
	TIMSK0 = (1<<TOIE0);
	
}

/****************************************/
// Interrupt routine
ISR(PCINT1_vect) {
	if (!(PINC & (1 << PORTC3))) {
		// Se inical el conteo regresivo
		if (contador == 0)
		{
			contador = 1; 
			cont = 5; 
			banTimer = 0; 
			//PORTD = DIS7SEG[cont]; 
			display7seg(cont); 
			decdJ1 = 0;
			decdJ2 = 0;
			J1 = 0;
			gana = 0; 
			PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB5));
			PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2)); 
		}
	}
	if ( (!(PINC & (1 << PORTC4))) && (cont == 0) && (gana == 0)) {
		// Botón primer jugador 
		decdJ1++; 
		if (decdJ1 == 1)
		{
			J1 = 1; 
			PORTB = (PORTB & 0xF0) | (J1 & 0x0F);
		}
		else if (decdJ1 == 2)
		{
			J1 = 2;
			PORTB = (PORTB & 0xF0) | (J1 & 0x0F);
		}
		else if (decdJ1 == 3)
		{
			J1 = 4;
			PORTB = (PORTB & 0xF0) | (J1 & 0x0F);
		}
		else if (decdJ1 >= 4)
		{
			J1 = 8;
			PORTB = (PORTB & 0xF0) | (J1 & 0x0F);
		}
		if ( J1 == 8)
		{
			gana = 1; 
			PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2));
			PORTB &= ~(1 << PORTB5);
			PORTB |= 0x0F;
			display7seg(1); 
		}
	}
	if ( (!(PINC & (1 << PORTC5))) && (cont == 0) && (gana == 0)) {
		// Botón segundo jugador
		decdJ2++;
		PORTC &= ~((1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2));
		PORTB &= ~(1 << PORTB5);
		if (decdJ2 == 1)
		{
			PORTC |= (1<<PORTC0);
		}
		else if (decdJ2 == 2)
		{
			PORTC |= (1<<PORTC1);
		}
		else if (decdJ2 == 3)
		{
			PORTC |= (1<<PORTC2);
		}
		else if (decdJ2 >= 4)
		{
			PORTB |= (1<<PORTB5);
		}
		if ( decdJ2 >= 4)
		{
			gana = 1; 
			PORTB &= 0xF0;
			PORTB |= (1 << PORTB5);
			PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2);
			display7seg(2);
		}
	}
}

ISR(TIMER0_OVF_vect){
	TCNT0 = 131;
	if (contador == 1)
	{
		banTimer++;
		
		if (banTimer >= 250 ){
			banTimer = 0; 
			cont--; 
			//PORTD = DIS7SEG[cont]; 
			display7seg(cont); 
			if (cont == 0)
			{
			contador = 0; 
			}
		}
	 
	}
}