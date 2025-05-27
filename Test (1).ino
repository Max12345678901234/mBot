#include "MeMCore.h"                       //Bibliothek für M-Bot initialisieren
#include <Wire.h>                          //Bibliothek für Bus System initialisieren

MeLEDMatrix ledMx(PORT_1);                 //LED-Matrix (Display) Modul an Port 1 angeschlossen
MeUltrasonicSensor ultraSensor(PORT_3);    //Ultraschallsensor an Port 3 angeschlossen

float winkelge;                            //Variable für Winkelgeschwindigkeit als Ganszahlig definiert
int abstand;                               //Variable für Abstand des Ultraschallsensors als Ganzzahlig definiert

float korre = 0.92;                        //Variable für rechten Korrekturfaktor als Dezimalzahl definiert
float korli = 1.0;                         //Variable für linken Korrektrurfaktor als dezimalzahl definiert
int speed = 120;                           //Variable für Geschwindigkeit als Integerzahl definiert und mit Startwert von 255 angegeben 

uint8_t links;                             //Ganzzahlige Variable mit 8 Bits für Geschwindigkeit des linken Motor
uint8_t rechts;                            //Ganzzahlige Variable mit 8 Bits für Geschwindigkeit des rechten Motor

MeDCMotor motorL(M1);                      //Motor links an Port M1 angeschlossen
MeDCMotor motorR(M2);                      //Motor rechts an Port M2 angeschlossen

MeGyro gyro;                               //Gyrosensor initialisiert mit Namen gyro

void setup()                               //Setup Schleife
{
  pinMode(7,INPUT);                        //interner Knopf an Port 7 angeschlossen
  ledMx.setBrightness(10);                 //Matrix LED Helligkeit von 10
  ledMx.setColorIndex(1);                  //Matrix LED zeigt Hintergund weiß und Zahlen in blau
  Serial.begin(115200);                    //Bus System startet
  gyro.begin();                            //Bus System sartet Gyro Sensor
}

void loop()                                //Arbeitsschleife startet
{ 
    gyro.update();                         //Gyro Sensor ansteuern
    Serial.read();                         //Gyro Sensor über Bus abfragen
    winkelge = gyro.getGyroY();            //Winkelgeschwindigkeit um Y-Achse messen und als Variable speichern
    ledMx.showNum(korre);                  //Winkelgeschwindigkeit auf LED-Matrix Modul anzeigen
    if(analogRead(7) < 100)                //Starte, wenn Knopf gedrückt wird 
    {
      delay(100);                          //Warte 100ms
      gyro.update();                       //Gyrosensor ansteuern
      Serial.read();                       //Gyrosensor lesen
      winkelge = gyro.getGyroY();          //Winkelgeschwindigkeit um Y-Achse gemessen und als Variable gespeichert                
      abstand = ultraSensor.distanceCm();  //Ultraschallsensor auslesen und Abstand in cm als Variable speichern
      motorR.run(15);                      //Rechten Motor ansteuern (dreht noch nicht!)
      motorL.run(-15);                     //Linken Motor ansteuern (dreht noch nicht!)
      ledMx.showNum(korre);                //Korrekturfaktor für Motor rechts auf LED Matrix Modul anzeigen 
      delay(2000);                         //Warte 2s
    while(abstand>20)                      //Führe Schleife aus, wenn Ultraschallsensor Abstand größer 20cm misst
    {
      links=speed*korli;                   //Geschwindigkeit linker Motor mit Korrekturfaktor angepasst
      rechts=speed*korre;                  //Geschwindigkeit rechter Motor mit Korrekturfaktor angepasst
      motorR.run(rechts);                  //Motor rechts dreht mit korregierter Geschwindigkeit
      motorL.run(-links);                  //Motor links dreht mit korregierter Geschwindigkeit
      delay(100);                          //Warte 100ms
      gyro.update();                       //Gyro Sensor ansteuern
      Serial.read();                       //Gyro Sensor über Bus abfragen
      winkelge = gyro.getGyroY();          //Winkelgeschwindigkeit um Y-Achse messen und als Variable speichern
      delay(100);                          //Warte 100ms
      ledMx.showNum(korre);                //Korrekturfaktor rechts auf LED-Matrix Modul anzeigen
      if (-5<winkelge<5){                  //Bedingung: Winkelgeschwindigkeit zwischen -5 und 5 (alle anderen Werte werden nicht beachtet, da fehlerbehaftet)
        korre = korre+0.0025*winkelge;     //Korrekturfaktor rechts wird mit Faktor über Winkelgeschwindigkeit angepasst, so dass Winkelgeschwindigkeit sinkt
      }                                    //Ende von Ausführung, wenn Bedingung in Zeile 60 erfüllt ist
      abstand = ultraSensor.distanceCm();  //Ultraschallsensor auslesen und Abstand in cm als Variable speichern
    }                                      //Starte ab Zeile 49 erneut, wenn Bedingung aus Zeile 48 erfüllt ist
  motorL.stop();                           //Motor links wird gestoppt    
  motorR.stop();                           //Motor rechts wird gestoppt
  delay(20);                               //Warte 20ms
}                                          //Ende von Ausführung, wenn Bedingung in Zeile 37 erfüllt ist
}                                          //Starte ab Zeile 32 erneut 

