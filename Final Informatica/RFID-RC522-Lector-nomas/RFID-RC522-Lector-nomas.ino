#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9    // Pin de reinicio
#define SS_PIN          10   // Pin de selección (SS/SDA)
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crear instancia de MFRC522

void setup() {
  Serial.begin(9600); // Iniciar comunicación serie
  SPI.begin();        // Iniciar SPI
  mfrc522.PCD_Init(); // Iniciar el módulo RC522

  Serial.println("Acerca una tarjeta RFID...");
}

void loop() {
  // Buscar tarjetas RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Mostrar el UID (identificador único) de la tarjeta
    Serial.print("UID de la tarjeta: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();
    mfrc522.PICC_HaltA(); // Detener la tarjeta actual
  }
}