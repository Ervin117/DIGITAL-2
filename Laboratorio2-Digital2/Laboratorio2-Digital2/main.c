/*
 * Laboratorio2-Digital2.c
 *
 * Created: 1/22/2026 11:51:28 AM
 * Author : Ervin Gomez 231226
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "LCD_display/LCD_DIS.h"
#include "ADC_Lib/ADC.h"
#include "UART/UARTLib.h"
/****************************************/
// Variables 
volatile uint16_t valor_adc7 = 0; 
volatile uint16_t valor_adc6 = 0; 
volatile uint8_t cont = 0; 
uint8_t nueva_opcion = 0;
uint8_t modo_cont = 0; 
uint8_t option = 0;
/****************************************/
// Function prototypes
void Mostrar_Voltaje_UART(uint16_t val_adc); 
void Mostrar_Decimal_UART(uint16_t dec); 
void Mostrar_Voltaje(uint16_t valor_adc); 
void Mostrar_Decimal(uint16_t decimal); 
void Mostrar_Contador(uint8_t cantidad); 
void conteo(char signo); 
void menu(); 
/****************************************/
// Main Function
int main(void)
{
	bits8INIT(); // Inicializar LCD
	initADC();   // Inicializar ADC
	serialUART(); //Inicializar UART
	
	 
	CMD_LCD(0x01); // Limpiar pantalla
	 _delay_ms(2);
	 
	// Escribir "S1"
	Cursor_LCD(2, 1); // Columna 1, Fila 1
	Write_Cad("S1:");
	 
	// Escribir "S2" 
	Cursor_LCD(7, 1); // Columna 7, Fila 1 
	Write_Cad("S2:");
	
	Cursor_LCD(12, 1); // Columna 12, Fila 1
	Write_Cad("S3:");
	
	sei(); 
	menu(); 
	while (1)
    {
		if (nueva_opcion)
		{
			nueva_opcion = 0; 
			if (option == '1')
			{
				modo_cont = 0; 
				serialString("\n Valor Pots \n");
				Mostrar_Voltaje_UART(valor_adc7);
				serialString("\n");
				Mostrar_Decimal_UART(valor_adc6);
				serialString("\n");
			}
			
			else if (option == '2')
			{
				modo_cont = 1;
				serialString("\n Contador: \n");
			}
			
			else if(modo_cont == 1)
			{
				if (option == '+')
				{
					conteo('+');
				}
				else if (option == '-')
				{
					conteo('-');
				}
			}
			
			else
			{
				menu();
			}
		}
		
		
		// MOSTRAR CANAL 6
		Cursor_LCD(1, 2); // Debajo de S1
		Mostrar_Voltaje(valor_adc7);

		// MOSTRAR CANAL 7
		Cursor_LCD(7, 2); // Debajo de S2
		Mostrar_Decimal(valor_adc6); 
		
		Cursor_LCD(13,2); 
		Mostrar_Contador(cont); 
		
		// Refresco de pantalla
		_delay_ms(100);
		
		
    }
}

void menu()
{
	serialString("\n Menu:\n");
	serialString("1. Leer Potenciometros\n");
	serialString("2. Contador Terminal a Display\n");
	serialString("Seleccione una opcion:\n");
}

void Mostrar_Voltaje(uint16_t valor_adc)
{
	 uint16_t voltaje_temp = ((uint32_t)valor_adc * 500UL) / 1024UL;
	 
	 // Extracción de dígitos
	 uint8_t entero = voltaje_temp / 100;     
	 uint8_t resto  = voltaje_temp % 100;      
	 uint8_t dec1   = resto / 10;              
	 uint8_t dec2   = resto % 10;              
	 
	 // Imprimir en LCD
	 Write_Carac(entero + '0'); 
	 Write_Carac('.');         
	 Write_Carac(dec1 + '0');   
	 Write_Carac(dec2 + '0');  
	 Write_Cad("V ");
	 
}

void Mostrar_Voltaje_UART(uint16_t val_adc)
{
	uint16_t voltaje_temp = ((uint32_t)val_adc * 500UL) / 1024UL;
	
	// Extracción de dígitos
	uint8_t entero = voltaje_temp / 100;
	uint8_t resto  = voltaje_temp % 100;
	uint8_t dec1   = resto / 10;
	uint8_t dec2   = resto % 10;
	
	// Imprimir en LCD
	serialLet(entero + '0');
	serialLet('.');
	serialLet(dec1 + '0');
	serialLet(dec2 + '0');
	serialLet('V');
	
}

void Mostrar_Decimal(uint16_t decimal)
{
	// Extraer cada dígito
	uint8_t miles    = (decimal / 1000);
	uint8_t centenas = (decimal / 100) % 10;
	uint8_t decenas  = (decimal / 10) % 10;
	uint8_t unidades = (decimal % 10);
	
	// Imprimir en LCD
	Write_Carac(miles + '0');
	Write_Carac(centenas + '0');
	Write_Carac(decenas + '0');
	Write_Carac(unidades + '0');
	
}

void Mostrar_Decimal_UART(uint16_t dec)
{
	// Extraer cada dígito
	uint8_t miles    = (dec / 1000);
	uint8_t centenas = (dec / 100) % 10;
	uint8_t decenas  = (dec / 10) % 10;
	uint8_t unidades = (dec % 10);
	
	// Imprimir en LCD
	serialLet(miles + '0');
	serialLet(centenas + '0');
	serialLet(decenas + '0');
	serialLet(unidades + '0');
	
}

void Mostrar_Contador(uint8_t cantidad)
{
	if (cantidad >= 10)
	{
		Write_Carac((cantidad/10) +'0');
	}
	Write_Carac((cantidad % 10) + '0');
}

void conteo(char signo)
{
	if (signo == '+')
	{
		if (cont <10)
		{
			cont++; 
		}
		else 
			cont = 0; 
	}
	if (signo == '-')
	{
		if (cont > 0)
		{
			cont--;
		}
		else
		cont = 10;
	}
	//Mostrar_Contador(cont); 
}


ISR(ADC_vect)
{
	uint8_t currentADC = ADMUX & 0x07; 
	uint16_t temp = ADC; 
	if (currentADC == 7)
	{
		valor_adc7 = temp; 
		ADMUX = 0; 
		ADMUX |= (1<<REFS0); // | (1<<ADLAR); 
		ADMUX |= (1<<MUX1) | (1<<MUX2); 
	}
	
	if (currentADC == 6)
	{
		valor_adc6 = temp; 
		ADMUX = 0;
		ADMUX |= (1<<REFS0); // | (1<<ADLAR);
		ADMUX |= (1 << MUX0) |  (1 << MUX1) | (1<<MUX2);
	}
	
	ADCSRA |= (1<<ADSC);
}

ISR(USART_RX_vect) 
{
	option = UDR0; 
	nueva_opcion = 1; 
}