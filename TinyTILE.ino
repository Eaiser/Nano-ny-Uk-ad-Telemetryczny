
#include "CurieIMU.h"
#include <Wire.h>
#include <math.h>


void setup() {
  
 Serial.begin(9600); 
 Wire.begin(); 

while (!Serial);   

  CurieIMU.begin();
  CurieIMU.setAccelerometerRange(8);
}

void loop() {
   float ax, ay, az;   
  
    
  CurieIMU.readAccelerometerScaled(ax, ay, az);
  float vectorModule = sqrt(pow(ax,2) + pow(ay,2) + pow(az,2));
  if (vectorModule < 1){
    vectorModule = 1;
  }
  
   char bufModule[4];
   dtostrf(vectorModule, 2, 2, bufModule);
   char bufx[5];
   dtostrf(ax, 3, 2, bufx);
   char bufy[5];
   dtostrf(ay, 3, 2, bufy);
   char bufz[5];
   dtostrf(az, 3, 2, bufz);
   char s1[]=" ";
  
     
    
   
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(bufModule, 4);
  Wire.write(s1, 1);
  Wire.write(bufx, 5);
  Wire.write(s1, 1);
  Wire.write(bufy, 5);
  Wire.write(s1, 1);
  Wire.write(bufz, 6);// sends one byte
  Wire.endTransmission();    // stop transmitting

delay(20);
}
