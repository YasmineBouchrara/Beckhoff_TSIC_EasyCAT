# Beckhoff TSIC EasyCAT

Arduino-Projekt zur Erfassung von vier TSIC716-Temperatursensoren und zur Übertragung der Messwerte über EasyCAT/EtherCAT an eine Beckhoff-Steuerung.

## Funktion

Das Programm liest vier TSIC716-Temperatursensoren aus. Die gemessenen Temperaturwerte werden direkt in den EasyCAT-Eingangspuffer geschrieben, damit eine Beckhoff-Steuerung die Werte über EtherCAT weiterverarbeiten kann.

Zusätzlich werden die aktuellen Messwerte über den Serial Monitor ausgegeben, damit die Sensorwerte während des Betriebs kontrolliert werden können.

## Hardware

- Arduino-kompatibles Board
- EasyCAT Shield / EtherCAT-Schnittstelle
- 4x TSIC716 Temperatursensoren
- Beckhoff CX8190 oder kompatible EtherCAT-Master-Steuerung

## Sensoren

| Sensor | Bedeutung |
|---|---|
| TSIC_1 | Umgebungstemperatur |
| TSIC_2 | Präzisionsbecken |
| TSIC_3 | Hauswasser |
| TSIC_4 | Luft / Air |

## Pinbelegung

| Sensor | Signal-Pin | VCC-Pin |
|---|---:|---:|
| TSIC_1 | 1 | 0 |
| TSIC_2 | 3 | 2 |
| TSIC_3 | 5 | 4 |
| TSIC_4 | 7 | 6 |

## Änderungen gegenüber der alten Version

- Reduzierung von 8 auf 4 aktive TSIC-Sensoren
- Anpassung der Pinbelegung an den aktuellen Hardwareaufbau
- Entfernen der alten Mittelwert- und Sortierfunktionen
- Direkte Übergabe der gemessenen Temperaturwerte an den EasyCAT-Puffer
- Aktivierung der seriellen Ausgabe zur Kontrolle der Messwerte
- Einfügen kurzer Verzögerungen nach jeder Sensorabfrage

## EasyCAT-Konfiguration

Das Projekt verwendet den EasyCAT Custom Mode:

```cpp
#define CUSTOM
#include "EasyCAT_TSIC.h"
#include "EasyCAT.h"
```

Die Datei `EasyCAT_TSIC.h` ist auf vier Eingangswerte ausgelegt:

| PDO-Eingang | Datentyp | Bedeutung |
|---|---|---|
| TSIC_1 | float | Umgebungstemperatur |
| TSIC_2 | float | Präzisionsbecken |
| TSIC_3 | float | Hauswasser |
| TSIC_4 | float | Luft / Air |

Damit Arduino, EasyCAT und Beckhoff dieselbe Datenstruktur verwenden, müssen mit dem Easy Configurator passende Dateien erzeugt werden:

- `.h` Datei: liegt als `EasyCAT_TSIC.h` im Arduino-Projekt
- `.xml` Datei: in Beckhoff/TwinCAT bzw. im EtherCAT-Konfigurator verwenden
- `.bin` Datei: mit dem Easy Configurator auf die EasyCAT-EEPROM schreiben

Die Custom-Konfiguration besteht aus 0 Byte Output und 16 Byte Input, also vier `float`-Werten.

Die passende EasyConfigurator-Projektdatei liegt unter:

```text
EasyConfigurator/Beckhoff_TSIC_EasyCAT.prj
```

Diese Datei kann im Easy Configurator geöffnet werden, um die zugehörigen `.h`, `.xml` und `.bin` Dateien zu erzeugen.

## Projektdateien

- `Beckhoff_V2.ino`
- `EasyCAT.h`
- `EasyCAT_TSIC.h`
- `EasyConfigurator/Beckhoff_TSIC_EasyCAT.prj`
- `TSIC.h`
- `TSIC.cpp`

## Hinweis

Die Bibliotheken liegen in diesem Projektordner. Beim Öffnen in der Arduino-IDE sollte der gesamte Ordner `Beckhoff_V2` geöffnet werden, damit die lokalen Header-Dateien gefunden werden.
