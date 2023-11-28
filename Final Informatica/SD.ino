#include <SPI.h>
#include <SD.h>

#define SSpin 10

File archivo;

void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando tarjeta...");

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

void loop()
 {
  // Tu lógica principal del programa va aquí (si tienes alguna)
}
