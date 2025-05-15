
#include "MeMCore.h"
#include <Wire.h>

MeLEDMatrix ledMx(PORT_1);
MeUltrasonicSensor ultraSensor(PORT_3);

int winkelge;
int abstand;

float korre = 1.0;
float korli = 1.0;
int speed = 120;

uint8_t links;
uint8_t rechts;

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

MeGyro gyro;

void setup()
{
  pinMode(7,INPUT);
  ledMx.setBrightness(6);
  ledMx.setColorIndex(1);
  Serial.begin(115200);
  gyro.begin();
}

void loop()
{ 
    gyro.update();
    Serial.read();
    winkelge = gyro.getGyroY();
    ledMx.showNum(winkelge);
    if(analogRead(7) < 100)
    {
      delay(500);
      gyro.update();
      Serial.read();
      winkelge = gyro.getGyroY(); 
      ledMx.showNum(korli);
      abstand = ultraSensor.distanceCm();
      delay(500);
    while(abstand>20)
    {
      links=speed*korli;
      rechts=speed*korre;
      motor2.run(rechts);
      motor1.run(-links);
      delay(250);
      gyro.update();
      Serial.read();
      winkelge = gyro.getGyroY();
      delay(250);
      ledMx.showNum(korre);
      if(winkelge>0){
        korre=korre+0.01;
      }
      if(winkelge<0){
        korre=korre-0.01;
      }
      abstand = ultraSensor.distanceCm();
    }
  motor1.stop();
  motor2.stop();
  delay(20);
}
}

