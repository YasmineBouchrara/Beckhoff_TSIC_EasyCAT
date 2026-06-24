/**
 *  Beckhoff_V2.ino
 *
 *  TSIC-Temperaturerfassung mit EasyCAT/EtherCAT
 *
 *  Dieses Programm liest vier TSIC716-Temperatursensoren aus und
 *  uebertraegt die gemessenen Werte ueber EasyCAT/EtherCAT an eine
 *  Beckhoff-Steuerung.
 *
 *  Aktiver Aufbau:
 *    TSIC_1: Umgebungstemperatur
 *    TSIC_2: Praezisionsbecken
 *    TSIC_3: Hauswasser
 *    TSIC_4: Luft / Air
 *
 *  Aenderungen gegenueber der alten Version:
 *    - Reduzierung von 8 auf 4 aktive Sensoren
 *    - Anpassung der Pinbelegung an den aktuellen Hardwareaufbau
 *    - Entfernen der alten Mittelwert-/Sortierlogik
 *    - Direkte Uebergabe der aktuellen Temperaturwerte an EasyCAT
 *    - Ausgabe der Messwerte ueber den Serial Monitor
 */
// Bibliotheken
#define CUSTOM
#include "C:\Users\yasmi\Desktop\Beckhoff_V2\EasyCAT_TSIC.h"
#include "C:\Users\yasmi\Desktop\Beckhoff_V2\TSIC.h"                     // Eigene TSIC-Bibliothek mit integrierten Korrekturfaktoren
#include "C:\Users\yasmi\Desktop\Beckhoff_V2\EasyCAT.h"
#include <SPI.h>

EasyCAT EASYCAT;

// TSIC-Pinbelegung: TSIC(SignalPin, VCCPin)
// Sensor von vorne: Signal in der Mitte, VCC links, abgerundete Seite oben.
TSIC Sensor1(1, 0);               // TSIC_1 - Umgebungstemperatur
TSIC Sensor2(3, 2);               // TSIC_2 - Praezisionsbecken
TSIC Sensor3(5, 4);               // TSIC_3 - Hauswasser
TSIC Sensor4(7, 6);               // TSIC_4 - Luft / Air
//TSIC Sensor5(1, 0);               // TSIC_5 - Maschine / Innenraum, in dieser Version deaktiviert
//TSIC Sensor6(1, 0);               // TSIC_6 - frei, in dieser Version deaktiviert
//TSIC Sensor7(1, 0);               // TSIC_7 - frei, in dieser Version deaktiviert
//TSIC Sensor8(1, 0);               // TSIC_8 - frei, in dieser Version deaktiviert

//========================== Variablen fuer Temperaturerfassung
uint16_t temperature = 0;
float Temperatur_C = 0;  float Temperatur_C1 = 0;  float Temperatur_C2 = 0;  float Temperatur_C3 = 0;  float Temperatur_C4 = 0;  float Temperatur_C5 = 0;  float Temperatur_C6 = 0;  float Temperatur_C7 = 0;  float Temperatur_C8 = 0;
float Tcorr_1 = 0;  float Tcorr_2 = 0;  float Tcorr_3 = 0;  float Tcorr_4 = 0;  float Tcorr_5 = 0;  float Tcorr_6 = 0;  float Tcorr_7 = 0;  float Tcorr_8 = 0;

// Zuordnung der Temperaturwerte zu den EasyCAT-Eingangsdaten
/*Temperatur_C1 = EasyCAT.BufferIn.Cust.TSIC_1;
Temperatur_C2 = EASYCAT.BufferIn.Cust.TSIC_2;
Temperatur_C3 = EASYCAT.BufferIn.Cust.TSIC_3;
Temperatur_C4 = EASYCAT.BufferIn.Cust.TSIC_4;*/
//Temperatur_C5 = EASYCAT.BufferIn.Cust.TSIC_5;
//Temperatur_C6 = EASYCAT.BufferIn.Cust.TSIC_6;
//Temperatur_C7 = EASYCAT.BufferIn.Cust.TSIC_7;
//Temperatur_C8 = EASYCAT.BufferIn.Cust.TSIC_8;

unsigned long Millis = 0;
unsigned long PreviousMillis = 0;
unsigned long PreviousSaw = 0;
unsigned long PreviousCycle = 0;


void setup()
{
    Serial.begin (115200);           // Serielle Ausgabe fuer Kontrolle im Serial Monitor

    Serial.print ("\nEasyCAT - Generic EtherCAT slave\n");          // Startmeldung

    if (EASYCAT.Init() == true)                                     // EasyCAT initialisieren
    {                                                               // Initialisierung erfolgreich
      Serial.print ("initialized");                                 //
    }                                                               //

    else                                                            // Initialisierung fehlgeschlagen
    {                                                               // EasyCAT-Board wurde nicht erkannt
      Serial.print ("initialization failed");                       //
                                                                  // Haeufige Ursache: SPI-Chip-Select am Board
                                                                  // passt nicht zur Firmware-Konfiguration.
                                                                  //

    pinMode(13, OUTPUT);                                          // Fehleranzeige: Arduino-LED blinkt dauerhaft
                                                                  //
    while(1)                                                      //
    {                                                             //
      digitalWrite (13, LOW);                                     //
      delay(500);                                                 //
      digitalWrite (13, HIGH);                                    //
      delay(500);                                                 //
    }                                                             //
  }

  PreviousMillis = millis();

}

void loop()
{
  EASYCAT.MainTask();                                   // EasyCAT-Kommunikation zyklisch bearbeiten
  Temperaturerfassung_Kuehlung();             // Vier TSIC-Sensoren nacheinander auslesen
  Ausgabe_Mon();                              // Messwerte auf dem Serial Monitor ausgeben
}

//_________________________________________
//             Unterfunktionen
//_________________________________________

void Temperaturerfassung_Kuehlung()
{
 if (Sensor1.getTemperture(&temperature))
  {
    Temperatur_C = Sensor1.calc_Celsius(&temperature);
    Temperatur_C1 = Temperatur_C + Tcorr_1;
    EASYCAT.BufferIn.Cust.TSIC_1 = Temperatur_C1;
    delay(100);
  }
  if (Sensor2.getTemperture(&temperature))
  {
    Temperatur_C = Sensor2.calc_Celsius(&temperature);
    Temperatur_C2 = Temperatur_C + Tcorr_2;
    EASYCAT.BufferIn.Cust.TSIC_2 = Temperatur_C2;
    delay(100);
  }
  if (Sensor3.getTemperture(&temperature))
  {
    Temperatur_C = Sensor3.calc_Celsius(&temperature);
    Temperatur_C3 = Temperatur_C + Tcorr_3;
    EASYCAT.BufferIn.Cust.TSIC_3 = Temperatur_C3;
    delay(100);
  }
  if (Sensor4.getTemperture(&temperature))
  {
    Temperatur_C = Sensor4.calc_Celsius(&temperature);
    Temperatur_C4 = Temperatur_C + Tcorr_4;
    EASYCAT.BufferIn.Cust.TSIC_4 = Temperatur_C4;
    delay(100);
  }
}


void Ausgabe_Mon()
{
  Serial.print("TSIC_1: ");
  Serial.println(Temperatur_C1);
  Serial.print("TSIC_2: ");
  Serial.println(Temperatur_C2);
  Serial.print("TSIC_3: ");
  Serial.println(Temperatur_C3);
  Serial.print("TSIC_4: ");
  Serial.println(Temperatur_C4);
  //Serial.print("TSIC_5: ");
  //Serial.println(Temperatur_C5);
  //Serial.print("TSIC_6: ");
  //Serial.println(Temperatur_C6);
  //Serial.print("TSIC_7: ");
  //Serial.println(Temperatur_C7);
  //Serial.print("TSIC_8: ");
  //Serial.println(Temperatur_C8);
}
