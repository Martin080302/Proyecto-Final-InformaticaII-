// CONECCIONES:
// DS1302 CLK/SCLK --> 7
// DS1302 DAT/IO --> 6
// DS1302 RST/CE --> 5
// DS1302 VCC --> 5v
// DS1302 GND --> GND

#include <RtcDS1302.h>

ThreeWire myWire(6,7,5); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup () 
{
    Serial.begin(9600);
    Rtc.Begin();

    RtcDateTime FechayHora = RtcDateTime(__DATE__, __TIME__);
    printDateTime(FechayHora);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        // Causas comunes:
        //    1) primera vez que se corre el programa y el dispositivo no estaba corriendo todavía
        //    2) la batería del dispositivo es baja o no tiene
        Serial.println("¡RTC perdió la confianza en DateTime!");
        Rtc.SetDateTime(FechayHora);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC estaba protegido contra escritura, permitiendo escribir ahora");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC no estaba activo, a partir de ahora sí");
        Rtc.SetIsRunning(true);
    }
}

void loop () 
{
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    if (!now.IsValid())
    {
        // Causas Comunes:
        //    1) La baterìa del dispositivo es baja o no tiene y la energìa fue desconectada
        Serial.println("¡RTC perdió la confianza en DateTime!");
    }
    delay(1000); // 1 segundo
}

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt)
{
    char horario_y_dia[26];

    snprintf_P(horario_y_dia, 
            countof(horario_y_dia),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(horario_y_dia);
}