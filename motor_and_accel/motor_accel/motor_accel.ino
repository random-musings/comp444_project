
#include <Servo.h>
#include "Motor_Cmd.h"
#include "Wire.h"
#include "ADXL345lib.h"


Accelerometer acc;
bool accel_avail= false;
double pX,pY,pZ;
 
//commands
long currentTime =0;
#define MOTOR_CHECKING  500
#define ACCEL_CHECKING  10
long motor_time = 0;
long accel_time = 0;


void AccelerometerSetup(){
    pX = 0;
    pY = 0;
    pZ = 0;
    if (acc.begin(OSEPP_ACC_SW_ON) != 0)
    {
        Serial.println("Error connecting to accelerometer");
        accel_avail = false;
        return;
    }
    acc.setOffsets( -5,3,8);

    acc.setSensitivity(ADXL345_RANGE_PM2G);
    accel_avail = true;
    accel_time = millis();

}

void MotorSetup(){
    motor_time = millis();
    Serial.println(" enter '1' for forward");
    Serial.println(" enter '2' for backward");
    Serial.println(" enter '3' for stop");
    Serial.println(" enter '-20' for right 20");
    Serial.println(" enter '-60' for right 60");
    Serial.println(" enter '-90' for right 90");
    Serial.println(" enter '-180' for right 180");
    Serial.println(" enter '-360' for right 180");
    Serial.println(" enter '20' for left 20");
    Serial.println(" enter '60' for left 60");
    Serial.println(" enter '90' for left 90");
    Serial.println(" enter '180' for left 180");
    Serial.println(" enter '360' for left 180");
}

void setup() {
  Serial.begin(9600);
  currentTime = millis();
  AccelerometerSetup();
  MotorSetup();
}


void ProcessMotor()
{
   while(Serial.available()>0)
   {
      int command = Serial.parseInt();
      Serial.print("command :");
      Serial.println(command);
      switch(command){
        case 1: Forward(HALF_SPEED); break;
        case 2: Backward(HALF_SPEED);break;
        case 3: Stop();break;
       default:
        command = abs(command)>360?360:command;
        if(command<0){
          Turn(RIGHT,command);
        }else if (command>10){
          Turn(LEFT,command);
        }
        break;
      }
   }
  motor_time = millis();
}

void ProcessAccelData()
{
    double x,y,z;
    if(accel_avail)
    {
      if (acc.readGs(&x, &y, &z) != 0)
      {
          Serial.println("Failed to read accelerometer");
          return;
      }
      /*
      if( (abs(pX)- abs(x)) > 0.1){
        Serial.print("x changed: ");
        Serial.println(x);
      }
      */
       Serial.print(x);
       Serial.print(",");
       Serial.print(y);
       Serial.print(",");
       Serial.print(z);
       Serial.println("");
         

      if( abs((abs(pY)- abs(y))) >= 0.05){        
        Serial.print("COLLISION!");
        Serial.print("   DIFFERENCE  ");
        Serial.println(y);
      }
      /*if( abs((abs(pY)- abs(y))) >= 0.05){        
        Serial.print("y changed: ");
        Serial.print(pY);
        Serial.print(" to ");
        Serial.print(y);
        Serial.print("   DIFFERENCE  ");
        Serial.println((abs(pY)- abs(y)));
      }
      */
/*      if( (abs(pZ)- abs(z)) > 0.1){
        Serial.print("z changed: ");
        Serial.print(pZ);
        Serial.print(" to ");
        Serial.println(z);
      }
      */
     pZ = z;
     pY = y;
     pX = x;
    }
    accel_time = millis();
}

void loop() {
  
 if( (millis()-motor_time)> MOTOR_CHECKING)
 {
  ProcessMotor();
 }
 if( (millis() - accel_time)> ACCEL_CHECKING)
 {
  ProcessAccelData();
 }

}


