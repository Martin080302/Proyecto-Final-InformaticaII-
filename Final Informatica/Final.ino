#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SD.h>

#define RST_PIN         9    // Pin de reinicio
#define SS_PIN          10   // Pin de selección (SS/SDA)        
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crear instancia de MFRC522

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
File archivo;

const int pulsadorIngresoPin = 2;
const int pulsadorEgresoPin = 3;
int band;
 
void setup() {
    SPI.begin();        // Iniciar SPI
    mfrc522.PCD_Init(); // Iniciar el módulo RC522
    lcd.setBacklightPin(3, POSITIVE);
    lcd.setBacklight(HIGH);
    lcd.begin(16, 2);
    lcd.clear();
    
  

    pinMode(pulsadorIngresoPin, INPUT);
    pinMode(pulsadorEgresoPin, INPUT);
}

void loop() {
    lcd.setCursor(1, 0);
    lcd.print("Bienvenido Al Cuartel  ");
    lcd.setCursor(1, 1);    
    lcd.print("Pulse cualquier boton");
    lcd.scrollDisplayLeft();

    if (digitalRead(pulsadorIngresoPin) == HIGH) {
      band=0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ingreso");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Acerca la tarjeta");
      lcd.setCursor(0, 1);
      delay(200);
      while(band==0){
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        // Mostrar el UID (identificador único) de la tarjeta
        lcd.setCursor(0, 1);
        lcd.print("UID: ");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
           lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
           lcd.print(mfrc522.uid.uidByte[i], HEX);
          } 
        band=1;
        mfrc522.PICC_HaltA(); 
        }
      }  
      delay(5000);
      lcd.clear();
    }
    else if (digitalRead(pulsadorEgresoPin) == HIGH) {
      band=0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Egreso");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Acerca la tarjeta");
      lcd.setCursor(0, 1);
      delay(200);
      while(band==0){
        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        // Mostrar el UID (identificador único) de la tarjeta
        lcd.setCursor(0, 1);
        lcd.print("UID: ");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
           lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
           lcd.print(mfrc522.uid.uidByte[i], HEX);
          } 
        band=1;
        mfrc522.PICC_HaltA(); 
        }
      }  
      delay(5000);
      lcd.clear();
    }
    delay(700);
  }