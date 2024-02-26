#include <LiquidCrystal_I2C.h>  // Incluye la librería para controlar el LCD mediante I2C
#include <Wire.h>               // Incluye la librería para la comunicación I2C
#include <RTClib.h>             // Incluye la librería para controlar el módulo RTC

#define I2C_ADDR 0x27  // Dirección I2C del módulo LCD
#define LCD_COLS 16    // Número de columnas y filas del LCD
#define LCD_ROWS 2     // Número de filas del LCD
#define RST_PIN 7      // Pin de reset para el módulo RTC

// Inicializa el objeto LCD con la dirección, número de columnas y filas definidos
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);
RTC_DS3231 rtc;    // Crea un objeto para interactuar con el módulo RTC

int PULSADOR_INGRESO = 2;       // Pin del pulsador de ingreso
int PULSADOR_EGRESO = 3;        // Pin del pulsador de egreso
int PULSADOR_PROCESSING = 4;    // Pin del pulsador de procesamiento
int estadoPulsador1;             // Variable para almacenar el estado del pulsador de ingreso
int estadoPulsador2;             // Variable para almacenar el estado del pulsador de egreso
int estadoPulsador3;             // Variable para almacenar el estado del pulsador de procesamiento
int estado = 0;                  // Variable que indica el estado actual del sistema

void setup() {
  Serial.begin(9600);   // Inicializa la comunicación serial a 9600 baudios

  // Inicializa el LCD
  lcd.init();
  // Enciende la retroiluminación (opcional)
  lcd.backlight();
  // Limpia la pantalla del LCD
  lcd.clear();

  // Inicializa el módulo RTC DS3231
  rtc.begin();

  // Si el RTC perdió energía, lo ajusta automáticamente con la hora actual
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Configura los pines de los pulsadores como entradas
  pinMode(PULSADOR_INGRESO, INPUT);
  pinMode(PULSADOR_EGRESO, INPUT);
  pinMode(PULSADOR_PROCESSING, INPUT);
}

// Función para formatear la fecha y hora en el formato DD/MM/YYYY HH:MM
String formatDateTime(DateTime time) {
  return (time.day() < 10 ? "0" : "") + String(time.day()) + "/" + 
         (time.month() < 10 ? "0" : "") + String(time.month()) + "/" + 
         String(time.year(), DEC) + " " + 
         (time.hour() < 10 ? "0" : "") + String(time.hour()) + ":" + 
         (time.minute() < 10 ? "0" : "") + String(time.minute());
}

void loop() {
  // Obtiene la fecha y hora actual del RTC una vez por ciclo de loop
  DateTime now = rtc.now();

  // Verifica el estado de los pulsadores
  switch (estado) {
    case 0:
      // Espera hasta que se pulse uno de los botones
      while (estadoPulsador1 == LOW && estadoPulsador2 == LOW /*&& estadoPulsador3 == LOW*/) {
        estadoPulsador1 = digitalRead(PULSADOR_INGRESO);
        estadoPulsador2 = digitalRead(PULSADOR_EGRESO);
        estadoPulsador3 = digitalRead(PULSADOR_PROCESSING);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.println("Bienvenido   ");

        // Muestra la fecha y hora en el LCD
        lcd.setCursor(0, 1);
        lcd.print(formatDateTime(now));

        // Espera 1 segundo antes de la próxima lectura de la hora
        delay(1000);

        // Actualiza la fecha y hora obteniendo la nueva fecha y hora del RTC
        now = rtc.now();
      }
      // Si se pulsa un botón, cambia al estado correspondiente
      if (estadoPulsador1 == HIGH) {
        estado = 1;
      } else if (estadoPulsador2 == HIGH) {
        estado = 2;
      } else if (estadoPulsador3 == HIGH) {
        estado = 3;
      }
      break;
    case 1:
      // Muestra "Ingreso" en el LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ingreso");

      // Muestra la fecha y hora actual en el LCD
      Serial.println("Ingreso: " + formatDateTime(now) + "_");
      lcd.setCursor(0, 1);
      lcd.print(formatDateTime(now));
      // Espera 3 segundos antes de volver al estado 0
      delay(3000);

      // Reinicia el estado de los pulsadores
      estadoPulsador1 = LOW;
      estadoPulsador2 = LOW;
      estadoPulsador3 = LOW;
      estado = 0;
      break;
    case 2:
      // Muestra "Egreso" en el LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Egreso");

      // Muestra la fecha y hora actual en el LCD
      Serial.println("Egreso: " + formatDateTime(now) + "_");
      lcd.setCursor(0, 1);
      lcd.print(formatDateTime(now));
      // Espera 3 segundos antes de volver al estado 0
      delay(3000);

      // Reinicia el estado de los pulsadores
      estadoPulsador1 = LOW;
      estadoPulsador2 = LOW;
      estadoPulsador3 = LOW;
      estado = 0;
      break;
    case 3:
      // Muestra "Añadir o Quitar" y "Personas" en el LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Añadir o Quitar");
      lcd.setCursor(0, 1);
      lcd.print("Personas");
      // Espera 1 segundo antes de volver al estado 0
      delay(1000);
      estado = 0;
      break;
  }
}

