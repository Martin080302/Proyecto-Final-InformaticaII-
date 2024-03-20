#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <MFRC522.h>

#define I2C_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2
#define RST_PIN 7
#define SS_PIN 10
#define RST_PIN1 9

MFRC522 mfrc522(SS_PIN, RST_PIN1);
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);
RTC_DS3231 rtc;

int PULSADOR_INGRESO = 2;
int PULSADOR_EGRESO = 3;
int PULSADOR_PROCESSING = 4;
int estadoProcessing = 0;
int RELE = 8;
int estadoPulsador1;
int estadoPulsador2;
int estadoPulsador3;
int estado = 0;
bool ingreso = false;
bool egreso = false;

void setup() {

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  rtc.begin();

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!mfrc522.PCD_PerformSelfTest()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error en MFRC522");
    while (1)
      ;
  }

  pinMode(PULSADOR_INGRESO, INPUT);
  pinMode(PULSADOR_EGRESO, INPUT);
  pinMode(PULSADOR_PROCESSING, INPUT);
  pinMode(RELE, OUTPUT);
}

String formatDateTime(DateTime time) {
  return (time.day() < 10 ? "0" : "") + String(time.day()) + "/" + (time.month() < 10 ? "0" : "") + String(time.month()) + "/" + String(time.year(), DEC) + " " + (time.hour() < 10 ? "0" : "") + String(time.hour()) + ":" + (time.minute() < 10 ? "0" : "") + String(time.minute());
}

void loop() {

  DateTime now = rtc.now();
  switch (estado) {
    case 0:
      estadoPulsador1 = LOW;
      estadoPulsador2 = LOW;
      estadoPulsador3 = LOW;
      delay(150);
      while (estadoPulsador1 == LOW && estadoPulsador2 == LOW && estadoPulsador3 == LOW) {
        estadoPulsador1 = digitalRead(PULSADOR_INGRESO);
        estadoPulsador2 = digitalRead(PULSADOR_EGRESO);
        estadoPulsador3 = digitalRead(PULSADOR_PROCESSING);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.println("Bienvenido   ");
        lcd.setCursor(0, 1);
        lcd.print(formatDateTime(now));
        delay(1000);
        now = rtc.now();
      }
      if (estadoPulsador1 == HIGH) {
        estado = 1;
      } else if (estadoPulsador2 == HIGH) {
        estado = 2;
      } else if (estadoPulsador3 == HIGH) {
        estado = 3;
      }
      break;
    case 1:
      ingreso = false;
      estadoPulsador1 = LOW;
      estadoPulsador2 = LOW;
      estadoPulsador3 = LOW;
      delay(1500);
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Ingreso");
      lcd.setCursor(0, 1);
      lcd.print("Acerca tarjeta");

      while (estadoPulsador1 == LOW && ingreso == false) {
        estadoPulsador1 = digitalRead(PULSADOR_INGRESO);

        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("Tarjeta");
          lcd.setCursor(3, 1);
          lcd.print("detectada");
          delay(1100);

          String UID = "";
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            UID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
            UID.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          UID.toUpperCase();

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("UID:" + UID);
          delay(3000);

          Serial.println("Ingreso: " + formatDateTime(now) + "_" + UID + "?");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fecha y Hora:");
          lcd.setCursor(0, 1);
          lcd.print(formatDateTime(now));
          delay(1500);
          ingreso = true;
        }
        delay(1000);
      }
      estadoPulsador1 = LOW;
      estadoPulsador2 = LOW;
      estadoPulsador3 = LOW;
      estado = 0;
      break;
    case 2:
      egreso = false;
      estadoPulsador1 = LOW;
      estadoPulsador2 = LOW;
      estadoPulsador3 = LOW;
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Egreso");
      lcd.setCursor(0, 1);
      lcd.print("Acerca tarjeta");

      while (estadoPulsador2 == LOW && egreso == false) {
        estadoPulsador2 = digitalRead(PULSADOR_EGRESO);

        if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
          lcd.clear();
          lcd.setCursor(4, 0);
          lcd.print("Tarjeta");
          lcd.setCursor(3, 1);
          lcd.print("detectada");
          delay(1500);

          String UID = "";
          for (byte i = 0; i < mfrc522.uid.size; i++) {
            UID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
            UID.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          UID.toUpperCase();

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("UID:" + UID);
          delay(1100);

          Serial.println("Egreso: " + formatDateTime(now) + "_" + UID + "?");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Fecha y Hora:");
          lcd.setCursor(0, 1);
          lcd.print(formatDateTime(now));
          delay(1500);
          egreso = true;
        }
        delay(1500);
      }
      estadoPulsador1 = LOW;
      estadoPulsador2 = LOW;
      estadoPulsador3 = LOW;
      estado = 0;
      break;
    case 3:
      estadoPulsador3 = LOW;     // Restablecer el estado del pulsador
      digitalWrite(RELE, HIGH);  //Lineas tx y rx Abiertas.
      delay(500);
      switch (estadoProcessing) {
        case 0:

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Modo Processing");
          while (true) {
            if (Serial.available() > 0) {
              String reciboProcessing = Serial.readStringUntil('\n');
              if (strcmp(reciboProcessing.c_str(), "ingreso") == 0) {
                estadoProcessing = 1;
              }
              if (strcmp(reciboProcessing.c_str(), "eliminar") == 0) {
                estadoProcessing = 2;
              }
              if (strcmp(reciboProcessing.c_str(), "registro") == 0) {
                estadoProcessing = 3;
              }
              break;
            }
            estadoPulsador3 = digitalRead(PULSADOR_PROCESSING);
            if (estadoPulsador3 == HIGH) {
              estado = 0;  // Volver al estado inicial
              estadoProcessing = 0;
              digitalWrite(RELE, LOW);
              break;  // Salir del bucle while
            }
          }
          delay(100);
          break;
        case 1:
          // Limpiar la pantalla y mostrar el mensaje
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Agregar Personas");
          estadoPulsador3 = digitalRead(PULSADOR_PROCESSING);
          if (estadoPulsador3 == HIGH) {
            estadoProcessing = 0;
            break;
          }
          // Verificar si se recibió un Nombre de Processing
          if (Serial.available() > 0) {
            String Nombre = Serial.readStringUntil('\n');  // Leer el Nombre de Processing
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(Nombre);
            delay(2000);

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Acerque tarjeta");
            // Esperar hasta que se detecte una tarjeta
            while (!mfrc522.PICC_IsNewCardPresent()) {
              delay(100);
            }

            // Leer el UID de la tarjeta
            if (mfrc522.PICC_ReadCardSerial()) {
              lcd.clear();
              lcd.setCursor(4, 0);
              lcd.print("Tarjeta");
              lcd.setCursor(3, 1);
              lcd.print("detectada");
              delay(1500);

              String UID = "";
              for (byte i = 0; i < mfrc522.uid.size; i++) {
                UID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
                UID.concat(String(mfrc522.uid.uidByte[i], HEX));
              }
              UID.toUpperCase();

              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("UID:" + UID);
              delay(1500);

              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(Nombre + " " + UID);
              digitalWrite(RELE, LOW);
              delay(1000);
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Agregando...");
              Serial.print(Nombre + "-" + UID + "!");
              delay(3000);

              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("Agregado");
              delay(1500);
              estadoProcessing = 0;  // Volver al estado inicial
              break;                 // Salir del bucle while
            }
          }
          break;
        case 2:

          digitalWrite(RELE, LOW);
          lcd.clear();
          // Esperar hasta que se detecte una tarjeta
          while (!mfrc522.PICC_IsNewCardPresent()) {
            lcd.setCursor(0, 0);
            lcd.print("Eliminar Personas");
            lcd.setCursor(0, 1);
            lcd.print("Acerque tarjeta");
            delay(100);
            estadoPulsador3 = digitalRead(PULSADOR_PROCESSING);
            if (estadoPulsador3 == HIGH) {
              estadoProcessing = 0;
              break;
            }
          }

          // Leer el UID de la tarjeta
          if (mfrc522.PICC_ReadCardSerial()) {
            lcd.clear();
            lcd.setCursor(4, 0);
            lcd.print("Tarjeta");
            lcd.setCursor(3, 1);
            lcd.print("detectada");
            delay(1500);

            String UID = "";
            for (byte i = 0; i < mfrc522.uid.size; i++) {
              UID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
              UID.concat(String(mfrc522.uid.uidByte[i], HEX));
            }
            UID.toUpperCase();

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("UID:" + UID);
            delay(1500);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Eliminando...");
            Serial.print(UID + "-");
            delay(3000);

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Eliminado");
            estadoProcessing = 0;  // Volver al estado inicial
            break;                 // Salir del bucle while
          }
          break;
        case 3:
          {
            // Limpiar la pantalla y mostrar el mensaje
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Registro de personal");
            digitalWrite(RELE, LOW);
            delay(1000);
            Serial.print("Registro\n");
            estadoProcessing = 0;  // Volver al estado inicial
            while (Serial.available() == 0) {
              delay(100);
            }
            String registro = "";
            while (Serial.available() > 0) {
              registro = Serial.readStringUntil('\0');
            }
            digitalWrite(RELE, HIGH);
            delay(1000);
            Serial.print(registro);
          }
          break;
      }
  }
}
