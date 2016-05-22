#include <Servo.h>
#include <Wire.h>
#include "ADXL345lib.h"
#include "Motor_Cmd.h"

#define FLEXPIN 0
#define SAMPLE_SIZE 100
#define FLEX_CHECKIN 200
int toleranceMin =0;
int toleranceMax =0;
int calculateBase =1;
long currSampleSize=0;
long sampleTotal=0;
int flexAverage =0;
long lastFlexCheck=0;
int rotate_count =0;
void setup() {
  // put your setup code here, to run once:
  pinMode(0,INPUT);
  Serial.begin(9600);
  Serial.println("enter 1 to calculate base");
  lastFlexCheck = millis();
  
}

void setFlexAverages(int value ){
   Serial.print(sampleTotal);
   Serial.print(" + ");
   Serial.print(value);
   Serial.print(" = ");
  sampleTotal  = sampleTotal+value;
   Serial.println(sampleTotal);
  currSampleSize ++;
  if(toleranceMin==0 || value<toleranceMin ){
    toleranceMin = value;
   }
   if(toleranceMax==0 || value>toleranceMax ){
    toleranceMax = value;
  }
  
  if(currSampleSize>SAMPLE_SIZE){
    Serial.print(" sampleTotal = ");
    Serial.println(sampleTotal);
    Serial.print("curr sample size =");
    Serial.println(currSampleSize);
    flexAverage = sampleTotal/currSampleSize;
    calculateBase=0;
    currSampleSize=0;
    Serial.print("Average Reading ");
    Serial.println(flexAverage);
    Serial.print("Min,Max  ");
    Serial.print(toleranceMin);
    Serial.print(", ");
    Serial.println(toleranceMax);
  }
  
}

int toggleMotor = 0;

void loop() {
  
  int value =analogRead(FLEXPIN);
  if(toggleMotor==0){
    toggleMotor=1;
   Forward(FAST);
  }
  if(calculateBase == 1){
    setFlexAverages(value);
  }else{
    if( (value<(toleranceMin-10) || value >(toleranceMax+10))
      and ( (millis()-lastFlexCheck) > FLEX_CHECKIN)){
      Serial.print("rotations ");
      rotate_count++;
      Serial.println(rotate_count);
      lastFlexCheck = millis();
    }
    
    /*
    while(Serial.available()>0){
      toggleMotor = (( toggleMotor +1)%2);
      if(toggleMotor ==1){
        
      }else{
        Stop();
      }
     
      calculateBase = Serial.parseInt();
      Serial.print("value entered ");
      Serial.println(calculateBase);
      currSampleSize = 0;
      sampleTotal =0;
      toleranceMin =0;
      toleranceMax =0;
    }
  */
  }

  
}

