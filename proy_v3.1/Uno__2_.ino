#include <SPI.h>
#include <SD.h>

File UIDFile;
File regisFile;
File enviFile;

const int ledrojo = 5;
const int ledverde = 6;
const int ledazul = 7;

void setup() {
  Serial.begin(9600);
  SPI.begin();

  pinMode(ledrojo, OUTPUT);
  pinMode(ledverde, OUTPUT);
  pinMode(ledazul, OUTPUT);

  // Inicializar la tarjeta SD
  if (!SD.begin(4)) {
    digitalWrite(ledrojo, HIGH);
    delay(5000);
    digitalWrite(ledrojo, LOW);
    return;
  }
}

void loop() {

  while (Serial.available() > 0) {
    // Leer el dato recibido
    String dato = Serial.readStringUntil('\n');
    if (dato == "Registro") {
      delay(500);
      enviFile = SD.open("REGISTRO.txt", FILE_READ);
      if (enviFile) {
        digitalWrite(ledazul, HIGH);
        delay(500);
        digitalWrite(ledazul, LOW);
        while (enviFile.available()) {
          char cadena = enviFile.read();
          Serial.print(cadena);
          delay(10);
        }
      }
      enviFile.close();
      dato = "";
    }
    // Buscar el índice del delimitador "_"
    if (dato.indexOf("_") != -1) {  // Si se encuentra el delimitador "_"
      // Extraer la primera parte hasta el delimitador "_"
      String primeraParte = dato.substring(0, dato.indexOf("_"));

      // Buscar el índice del delimitador "?"
      if (dato.indexOf("?") != -1) {  // Si se encuentra el delimitador "?"
        // Extraer la segunda parte desde el delimitador "_" hasta el delimitador "?"
        String segundaParte = dato.substring(dato.indexOf("_") + 1, dato.indexOf("?"));

        // Eliminar espacios en blanco al principio de segundaParte
        segundaParte.trim();

        // Abrir el archivo correspondiente en la tarjeta SD
        UIDFile = SD.open(segundaParte + ".txt", FILE_READ);
        regisFile = SD.open("REGISTRO.txt", FILE_WRITE);
        if (UIDFile) {  // Si el archivo se abrió correctamente
          // Encender el LED de archivo abierto correctamente
          digitalWrite(ledverde, HIGH);

          // Leer y mostrar el contenido del archivo
          regisFile.print("");
          regisFile.print("Nombre: ");
          while (UIDFile.available()) {
            char caracter = UIDFile.read();
            Serial.write(caracter);  // Leer y escribir en el puerto serial
            regisFile.print(caracter);
          }
          regisFile.println(" " + primeraParte);
          UIDFile.close();  // Cerrar el archivo
          regisFile.close();  // Cerrar el archivo
        } else {
          // Encender el LED de error al abrir archivo
          digitalWrite(ledrojo, HIGH);
        }

        // Apagar los LEDs después de un tiempo
        delay(3000);
        digitalWrite(ledrojo, LOW);
        digitalWrite(ledverde, LOW);
      }
    }
    if (dato.indexOf("-") != -1) {
      String nombre = dato.substring(0, dato.indexOf("-"));
      if (dato.indexOf("!") != -1) {  // Si se encuentra el delimitador "?"
        // Extraer la segunda parte desde el delimitador "_" hasta el delimitador "?"
        String UID_agreg = dato.substring(dato.indexOf("-") + 1, dato.indexOf("!"));

        // Eliminar espacios en blanco al principio de segundaParte
        UID_agreg.trim();
        if (SD.exists(UID_agreg + ".txt")) {
          digitalWrite(ledrojo, HIGH);
          delay(2000);
          digitalWrite(ledrojo, LOW);
        } else {
          UIDFile = SD.open(UID_agreg + ".txt", FILE_WRITE);
          if (UIDFile) {
            UIDFile.print(nombre);
            UIDFile.close();
            digitalWrite(ledverde, HIGH);
          } else {
            digitalWrite(ledrojo, HIGH);
          }
          delay(2000);
          digitalWrite(ledrojo, LOW);
          digitalWrite(ledverde, LOW);
        }
      } else {
        if (SD.exists(nombre + ".txt")) {
          if (SD.remove(nombre + ".txt")) {
            digitalWrite(ledverde, HIGH);
            delay(2000);
            digitalWrite(ledverde, LOW);
          } else {
            digitalWrite(ledrojo, HIGH);
            delay(2000);
            digitalWrite(ledrojo, LOW);
          }
        } else {
          digitalWrite(ledrojo, HIGH);
          delay(2000);
          digitalWrite(ledrojo, LOW);
        }
      }
    }
  }
}
