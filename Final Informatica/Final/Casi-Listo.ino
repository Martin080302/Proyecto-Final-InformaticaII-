#include <SPI.h>       // Librería para comunicación SPI
#include <MFRC522.h>   // Librería para el lector RFID
#include <Wire.h>      // Librería para comunicación I2C
#include <Time.h>      // Librería para el tiempo 
#include <RTClib.h>    // Librería para el RTC DS3231
#include <SD.h>        // Librería para la memoria SD

#define RST_PIN 9 // Pin 9 para Reset del MFRC522 
#define SS_PIN 8  // Pin 8 para el selector de chip del MFRC522

const int pulsadorIngresoPin = 2;
const int pulsadorEgresoPin = 3;

File myFile;   

MFRC522 mfrc522(SS_PIN, RST_PIN); // Crear instancia de MFRC522

String read_rfid;   // Variable donde se almacena la tarjeta leída 
String tag[4] = {"4369d24", "707870a4", "a49839a5", "0"}; // Tags almacenados 
boolean P1=false, P2=false, P3=false, P4=false;    // Condiciones para los estados de entrada o salida 
String nombre[5] = {"Martin ", "Maria ", "Pedro ", "Sofia ", "Desconocido "}; // Usuarios registrados 

int pinSD = 10;    // Pin para seleccionar la SD 
int Led=7;    // Pin 7 para el LED verde 

RTC_DS3231 rtc;

void setup() {
  pinMode(Led, OUTPUT);   
  Serial.begin(9600);  // Iniciar la comunicación serial
  SPI.begin();            // Inicializar la comunicación SPI 
  mfrc522.PCD_Init();     // Inicializar el lector RFID 
  if (!SD.begin(pinSD)) { // Comprobar si la tarjeta SD está activa 
    Serial.println("Inicializacion fallida!");
    return;
  }
  Serial.println("Bienvenido Al Cuartel");
  Serial.println("Pulse cualquier boton");
  if (!rtc.begin()) {
    Serial.println("No se pudo encontrar el RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC perdió energía, ajustando la hora...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(pulsadorIngresoPin, INPUT);
  pinMode(pulsadorEgresoPin, INPUT);
}

// Función para leer los bloques de la tarjeta RFID 
void dump_byte_array(byte *buffer, byte bufferSize) {    
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid += String(buffer[i], HEX);
  }
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent())   // Comprobar si no existe ninguna tarjeta presente en el lector 
    return;
  
  if (!mfrc522.PICC_ReadCardSerial())     // Comprobar si no se ha leido la tarjeta 
    return;

  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size); // Llamar a la función para leer el código de la tarjeta 
  Personal(); // Llamar a la función para identificar a la persona
  registro();  // Entrar en el registro de evento 
  digitalWrite(Led, LOW); // Apagar el LED indicador 
  
  Serial.println("Bienvenido Al Cuartel");
  Serial.println("Pulse cualquier boton");
}

// Registro de evento
void registro() {
  myFile = SD.open("registro.txt", FILE_WRITE); // Abrir el archivo Registro donde se guardan los datos obtenidos 
  DateTime now = rtc.now();  // Obtener la fecha y hora actual del RTC

  myFile.print(", UID:" + read_rfid + ", Hora:" + print2digits(now.hour()) + ":" + print2digits(now.minute()) + ", Fecha(D/M/Y):" + print2digits(now.day()) + "/" + print2digits(now.month()) + "/" + now.year());
  Serial.print(", UID:" + read_rfid + ", Hora:" + print2digits(now.hour()) + ":" + print2digits(now.minute()) + ", Fecha(D/M/Y):" + print2digits(now.day()) + "/" + print2digits(now.month()) + "/" + now.year());

  myFile.println();
  Serial.println();
  
  myFile.close(); // Cerrar el archivo de la SD 
  digitalWrite(Led, HIGH); // Encender el LED verde 
  delay(500); // Esperar medio segundo 
}

// Asegurarse de imprimir dos dígitos
String print2digits(int numero) {
  if (numero < 10) { // Si el dígito es menor de 10
    return "0" + String(numero);
  }
  return String(numero);
}

// Identificación de personal
void Personal() {
  for (int i = 0; i < 4; i++) {
    if (tag[i] == read_rfid) { // Si el tag corresponde a uno de los usuarios
      Serial.print(nombre[i]); // Imprimir el nombre correspondiente
      myFile.print(nombre[i]);
      return; // Salir de la función una vez que se ha encontrado una coincidencia
    }
  }
    // Si no se encontró una coincidencia, imprimir "Desconocido" y escribir en el archivo
  Serial.print(nombre[4]);
  myFile.print(nombre[4]);
}

