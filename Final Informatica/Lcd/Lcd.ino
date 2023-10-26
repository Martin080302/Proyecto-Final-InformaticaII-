#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);

const int pulsadorIngresoPin = 2;
const int pulsadorReingresoPin = 3;
const int pulsadorEgresoPin = 4;

void setup() {
    lcd.setBacklightPin(3, POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.begin(16, 2);
    lcd.clear();
   


    pinMode(pulsadorIngresoPin, INPUT);
    pinMode(pulsadorReingresoPin, INPUT);
    pinMode(pulsadorEgresoPin, INPUT);
}

void loop() {
    
   
    lcd.setCursor(2, 0);
    lcd.print("Bienvenido Al Cuartel  ");
    lcd.setCursor(2, 1);    
    lcd.print("Pulse cualquier boton");
    lcd.scrollDisplayLeft();
    delay(400);

    if (digitalRead(pulsadorIngresoPin) == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ingreso");
        delay(2000);
        lcd.clear();
    }
    else if (digitalRead(pulsadorReingresoPin) == HIGH) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Reingreso");
        delay(2000);
        lcd.clear();
    }
    else if (digitalRead(pulsadorEgresoPin) == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Egreso");
        delay(2000);
        lcd.clear();
    }

    
    
  }