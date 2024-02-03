#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>

#define RST_PIN 9
#define SS_PIN 10
#define PIN_SD 4
#define LED_VERDE 7
#define PULSADOR_INGRESO 2
#define PULSADOR_EGRESO 3
#define SDA_MH A6
#define SCL_MH A7

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Ajusta los parámetros según tu LCD
MFRC522 mfrc522(SS_PIN, RST_PIN);
File myFile;

String read_rfid;  // Declaración de la variable fuera de la función

const char *Mes[13] = {" ", "Ene", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
const String tag[4] = {"4c8f7e6e", "707870a4", "a49839a5", "d7485d79"};
const String nombre[5] = {"Lean ", "Maria ", "Pedro ", "Sofia ", "Desconocido "};
boolean P1 = false, P2 = false, P3 = false, P4 = false;

void setup() {
  Wire.begin();
  pinMode(LED_VERDE, OUTPUT);
  lcd.begin(16, 2);  // Inicializa el LCD con 16 columnas y 2 filas
  lcd.setBacklight(HIGH);  // Apaga la luz de fondo inicialmente
  SPI.begin();
  mfrc522.PCD_Init();
  if (!SD.begin(PIN_SD)) {
    lcd.println("Inicializacion fallida en la tarjeta SD!");
    return;
  }
  lcd.println("Inicializacion lista.");

  pinMode(PULSADOR_INGRESO, INPUT);
  pinMode(PULSADOR_EGRESO, INPUT);
}

void loop() {
  lcd.setCursor(1, 0);
  lcd.print("Bienvenido Al Cuartel  ");
  lcd.setCursor(1, 1);
  lcd.print("Pulse cualquier boton");
  lcd.scrollDisplayLeft();
  delay(700);
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  registro();
  digitalWrite(LED_VERDE, LOW);
  
}

void dump_byte_array(byte *buffer, byte bufferSize) {
  read_rfid = "";  // Limpia el contenido anterior
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}

void registro() {
  myFile = SD.open("registro.txt", FILE_WRITE);
  tmElements_t tm;
  RTC.read(tm);
  ingreso();
  Serial.print(",");
  myFile.print(",");
  Personal();
  Serial.print(",");
  myFile.print(",");
  Serial.print("UID:");
  Serial.print(read_rfid);
  myFile.print(read_rfid);
  Serial.print(",");
  myFile.print(",");
  Serial.print(" Hora:");
  myFile.print(" Hora:");
  print2digits(tm.Hour);
  Serial.write(':');
  myFile.print(":");
  print2digits(tm.Minute);
  Serial.write(':');
  myFile.print(":");
  print2digits(tm.Second);
  Serial.print(",");
  myFile.print(",");
  Serial.print(" Fecha(D/M/Y):");
  myFile.print(" Fecha(D/M/Y):");
  Serial.print(tm.Day);
  myFile.print(tm.Day);
  Serial.write('/');
  myFile.print("/");
  Serial.print(Mes[tm.Month]);
  myFile.print(Mes[tm.Month]);
  Serial.write('/');
  myFile.print("/");
  Serial.print(tmYearToCalendar(tm.Year));
  myFile.print(tmYearToCalendar(tm.Year));
  Serial.println();
  myFile.println();
  myFile.close();
  digitalWrite(LED_VERDE, HIGH);
  delay(500);
}

void print2digits(int numero) {
  if (numero >= 0 && numero < 10) {
    Serial.write('0');
    myFile.print("0");
  }
  Serial.print(numero);
  myFile.print(numero);
}

void Personal() {
  if (tag[0] == read_rfid) {
    Serial.print(nombre[0]);
    myFile.print(nombre[0]);
  }
  if (tag[1] == read_rfid) {
    Serial.print(nombre[1]);
    myFile.print(nombre[1]);
  }
  if (tag[2] == read_rfid) {
    Serial.print(nombre[2]);
    myFile.print(nombre[2]);
  }
  if (tag[3] == read_rfid) {
    Serial.print(nombre[3]);
    myFile.print(nombre[3]);
  }
  if (tag[0] != read_rfid && tag[1] != read_rfid && tag[2] != read_rfid && tag[3] != read_rfid) {
    Serial.print(nombre[4]);
    myFile.print(nombre[4]);
  }
}

void ingreso() {
  bool x = false;
  if (tag[0] == read_rfid && P1 == false && x == false) {
    Serial.print("Entrada ");
    myFile.print("Entrada ");
    P1 = true;
    x = true;
  }
  if (tag[0] == read_rfid && P1 == true && x == false) {
    Serial.print("Salida ");
    myFile.print("Salida ");
    P1 = false;
    x = true;
  }
  if (tag[1] == read_rfid && P2 == false && x == false) {
    Serial.print("Entrada ");
    myFile.print("Entrada ");
    P2 = true;
    x = true;
  }
  if (tag[1] == read_rfid && P2 == true && x == false) {
    Serial.print("Salida ");
    myFile.print("Salida ");
    P2 = false;
    x = true;
  }
  if (tag[2] == read_rfid && P3 == false && x == false) {
    Serial.print("Entrada ");
    myFile.print("Entrada ");
    P3 = true;
    x = true;
  }
  if (tag[2] == read_rfid && P3 == true && x == false) {
    Serial.print("Salida ");
    myFile.print("Salida ");
    P3 = false;
    x = true;
  }
  if (tag[3] == read_rfid && P4 == false && x == false) {
    Serial.print("Entrada ");
    myFile.print("Entrada ");
    P4 = true;
    x = true;
  }
  if (tag[3] == read_rfid && P4 == true && x == false) {
    Serial.print("Salida ");
    myFile.print("Salida ");
    P4 = false;
    x = true;
  }
}
