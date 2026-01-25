/*
 * Laboratorio2-Digital2.c
 *
 * Created: 1/22/2026 11:51:28 AM
 * Author : razer
 */ 

#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#include "LCD_display/LCD_DIS.h"
#include "ADC_Lib/ADC.h"
/****************************************/
// Variables 
char buffer[16]; 
uint16_t val_adc6 = 0; 
uint16_t val_adc7 = 0; 

/****************************************/
// Function prototypes
void Mostrar_Voltaje(uint16_t valor_adc); 
/****************************************/
// Main Function
int main(void)
{
	 // Variables para guardar lectura cruda del ADC
	 uint16_t lectura_ch6 = 0;
	 uint16_t lectura_ch7 = 0;

	 bits8INIT(); // Inicializar LCD
	 initADC();   // Inicializar ADC
	 
	 // --- Configuración de etiquetas Fijas (Solo se escribe una vez) ---
	 CMD_LCD(0x01); // Limpiar pantalla
	 _delay_ms(2);
	 
	 // Escribir "S1" en la izquierda (Fila 1)
	 Cursor_LCD(1, 1); // Columna 1, Fila 1
	 Write_Cad("S1:");
	 
	 // Escribir "S2" más a la derecha (Fila 1)
	 Cursor_LCD(7, 1); // Columna 9, Fila 1 (mitad de pantalla)
	 Write_Cad("S2:");
	while (1)
    {
		// PROCESAR SENSOR 1 (Canal 6)
		lectura_ch6 = lecADC(6);
		Cursor_LCD(1, 2);
		Mostrar_Voltaje(lectura_ch6); 
		
		
		// PROCESAR SENSOR 2 (Canal 7)
		lectura_ch7 = lecADC(7);
		Cursor_LCD(7, 2);
		Mostrar_Voltaje(lectura_ch7); 
		_delay_ms(300);
    }
}

void Mostrar_Voltaje(uint16_t valor_adc)
{
	// Usamos uint32_t (long) para evitar desbordamiento en la multiplicación
	// 1023 * 500 = 511,500 (esto no cabe en 16 bits, por eso usamos 32 bits)
	uint32_t voltaje_temp = ((uint32_t)valor_adc * 500UL) / 1024UL;
	
	// Ahora voltaje_temp tiene un valor como 345 (que representa 3.45V)
	
	// 1. Extraer parte entera (Unidades)
	uint8_t entero = voltaje_temp / 100; // 345 / 100 = 3
	
	// 2. Obtener el resto para los decimales
	uint8_t resto = voltaje_temp % 100;  // 345 % 100 = 45
	
	// 3. Extraer primer decimal
	uint8_t dec1 = resto / 10;           // 45 / 10 = 4
	
	// 4. Extraer segundo decimal
	uint8_t dec2 = resto % 10;           // 45 % 10 = 5
	
	// --- Imprimir en LCD ---
	Write_Carac(entero + '0'); // Convertir número a carácter ASCII
	Write_Carac('.');
	Write_Carac(dec1 + '0');
	Write_Carac(dec2 + '0');
	//Write_Cad("V");
}
