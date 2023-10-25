#include <Wire.h>			// libreria de comunicacion por I2C
#include <LCD.h>			// libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>		// libreria para LCD por I2C

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7

void setup()
{
    lcd.setBacklightPin(3,POSITIVE);	// puerto P3 de PCF8574 como positivo
    lcd.setBacklight(HIGH);		// habilita iluminacion posterior de LCD
    lcd.begin(16, 2);			// 16 columnas por 2 lineas para LCD 1602A
    lcd.clear();			// limpia pantalla
}

void loop()
{
    lcd.setCursor(1, 0);		
    lcd.print("Bienvenido Al Cuartel");				
    lcd.setCursor(1, 1);		// ubica cursor en columna 2 y linea 0
    lcd.print("Pulse cualquier boton");			// escribe texto
    lcd.scrollDisplayLeft();		
    delay(400);	
}