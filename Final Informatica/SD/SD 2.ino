#include <SPI.h>
#include <SD.h>

const int SSpin = 8;
const int mosipin = 5;
const int misopin = 4;
const int sckpin = 7;

File archivo;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando tarjeta...");

  // Configuración de pines SPI
  pinMode(SSpin, OUTPUT);
  digitalWrite(SSpin, HIGH); // Asegurar que el pin SS está alto (deseleccionado)
  SPI.begin();
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0)); // Puedes ajustar la velocidad según tus necesidades

  if (!SD.begin(SSpin)) {
    Serial.println("¡Fallo en inicialización!");
    return;
  }

  Serial.println("Inicialización correcta");

  archivo = SD.open("prueba.txt", FILE_WRITE);
  if (archivo) {
    Serial.println("Escribiendo en archivo prueba.txt...");
    archivo.println("¡Hola, mundo!"); // Escribir algunos datos en el archivo
    archivo.close();
    Serial.println("Escritura correcta");
  } else {
    Serial.println("Error en apertura de prueba.txt");
  }

  archivo = SD.open("prueba.txt");
  if (archivo) {
    Serial.println("Contenido de prueba.txt:");
    while (archivo.available()) {
      Serial.write(archivo.read());
    }
    archivo.close();
  } else {
    Serial.println("Error en la apertura del archivo prueba.txt");
  }
}

void loop() {
  // Tu lógica principal del programa va aquí (si tienes alguna)
}

