#include <Wire.h>
#include "ADXL345lib.h"


Accelerometer acc;
bool fail;

void setup()
{
    Serial.begin(9600);
    
    // set the I2C address of the accelerometer
    //
    // With the OSEPP Accelerometer use:
    //      OSEPP_ACC_SW_ON    - Switch is in the ON position
    //      OSEPP_ACC_SW_OFF   - Switch is in the OFF position
    //
    // begin will return 0 on success
    if (acc.begin(OSEPP_ACC_SW_ON) != 0)
    {
        Serial.println("Error connecting to accelerometer");
        fail = true;
        return;
    }
    acc.setOffsets( -3,-3,7);
    acc.setSensitivity(ADXL345_RANGE_PM4G);
    
}


float currCount =0;
double avgX=0,avgY=0,avgZ=0;
#define MODIFIER  0.05;
#define MODIFIER_SAMPLE  0.95;
#define SAMPLESIZE 20
void getAverage(double x,double y,double z)
{
  if(currCount<SAMPLESIZE){
     currCount++;
     avgX = (avgX+x)/currCount; 
     avgY = (avgY+y)/currCount; 
     avgZ = (avgZ+z)/currCount; 
  }else{
    avgX = avgX*MODIFIER_SAMPLE +x* MODIFIER;
    avgY = avgY*MODIFIER_SAMPLE +y* MODIFIER;
    avgZ = avgZ*MODIFIER_SAMPLE +z* MODIFIER;
  }
}

void loop()
{
    // don't bother reading if we failed to connect
    if (fail){
        Serial.println("failed to setup");
        delay(1000);
        return;
    }

    // 3 doubles to store the data
    double x, y, z;

    // Read Gs
    // Note: You can also read the raw data by calling
    //       readRaw() - readRaw takes int16_t instead of doubles
    if (acc.readGs(&x, &y, &z) != 0)
    {
        Serial.println("Failed to read accelerometer");
        return;
    }
    getAverage(x,y,z);
    // print them out
         
        Serial.print("x,y,z: ");
          Serial.print(x);
          Serial.print("                  ");
          Serial.print(y);
         // Serial.print("                   ");
       //   Serial.print(z);
          Serial.println("");
  /*  Serial.print("X: ");
    Serial.print(avgX);
    Serial.print(" Y: ");
    Serial.print(avgY);
    Serial.print(" Z: ");
    Serial.println(avgZ);
    */

  delay(50);
}
