/**
 *  Programm für TSIC-Erfassung und Weiterleitung über EtherCAT an Beckhoff CX8190
 *  
 *  Präzisionskühlung
 *  Auslesen von 2x4 TSIC716 Sensoren und Mittelwertbildung.
 *  5 Temperaturwerte in Array, Sortierung und Medianbildung
 *  Rel. V1 = TSIC-Erfassung steht, Kommunikation EtherCAT noch nicht
 */
//Bibliotheken
#define CUSTOM
#include "C:\Users\yasmi\Desktop\Beckhoff_V2\EasyCAT_TSIC.h"
#include "C:\Users\yasmi\Desktop\Beckhoff_V2\TSIC.h"                     //eigene TSIC Bibliothek, da eigene Korrekturfaktoren hardcoded integriert sind
#include "C:\Users\yasmi\Desktop\Beckhoff_V2\EasyCAT.h"
#include <SPI.h>

EasyCAT EASYCAT; 

//Wasserregelung TSIC [SignalPin 2 mitte, VCCPin 3 links] rundung oben, beinchen nach hinten //#### Pins (27, 26) defekt! -> dritter Sensorsteckplatz defekt!
TSIC Sensor1(1, 0);               // 1. Sensor Präzisionskühlung - Umgebungstemperatur
TSIC Sensor2(3, 2);               // 2. Sensor Präzisionskühlung - Präzisionsbecken
TSIC Sensor3(5, 4);               // 3. Sensor Präzisionskühlung - Hauswasser
TSIC Sensor4(7, 6);               // 4. Sensor Präzisionskühlung - Air 
//TSIC Sensor5(1, 0);               // 5. Sensor Maschine - Innenraum     ###### Pinbelegung nicht richtig
//TSIC Sensor6(1, 0);               // 6. Sensor Maschine - frei          ###### Pinbelegung nicht richtig
//TSIC Sensor7(1, 0);               // 3. Sensor Maschine - frei          ###### Pinbelegung nicht richtig
//TSIC Sensor8(1, 0);               // 4. Sensor Maschine - frei          ###### Pinbelegung nicht richtig

//==========================Variablen für Temperaturerfassung
uint16_t temperature = 0;
float Temperatur_C = 0;  float Temperatur_C1 = 0;  float Temperatur_C2 = 0;  float Temperatur_C3 = 0;  float Temperatur_C4 = 0;  float Temperatur_C5 = 0;  float Temperatur_C6 = 0;  float Temperatur_C7 = 0;  float Temperatur_C8 = 0;
float Tcorr_1 = 0;  float Tcorr_2 = 0;  float Tcorr_3 = 0;  float Tcorr_4 = 0;  float Tcorr_5 = 0;  float Tcorr_6 = 0;  float Tcorr_7 = 0;  float Tcorr_8 = 0;

//Variablen für EASYCAT
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
    Serial.begin (115200);           //1. Serielle Datenübertragung
    
    Serial.print ("\nEasyCAT - Generic EtherCAT slave\n");          // print the banner 
  
    if (EASYCAT.Init() == true)                                     // initialization
    {                                                               // succesfully completed
      Serial.print ("initialized");                                 //
    }                                                               //
  
    else                                                            // initialization failed   
    {                                                               // the EasyCAT board was not recognized
      Serial.print ("initialization failed");                       //     
                                                                  // The most common reason is that the SPI 
                                                                  // chip select choosen on the board doesn't 
                                                                  // match the one choosen by the firmware
                                                                  
    pinMode(13, OUTPUT);                                          // stay in loop for ever
                                                                  // with the Arduino led blinking
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
  EASYCAT.MainTask();                                   // execute the EasyCAT task
  Temperaturerfassung_Kuehlung();             //Auslesen der Kühlung TSIC-Sensoren nacheinander
  Ausgabe_Mon();                              //Ausgabe auf Serial Monitor
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
