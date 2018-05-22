 /*
  * File  : DFRobot_MPU6050_Accel_pitch_roll.ino
  * Brief : This is MPU6050 Triple Axis Gyroscope & Accelerometer. 
  *         Pitch & Roll Accelerometer Example.
  *         After initialization MPU6050 gives the value of Pitch and Roll.
  */

#include <Wire.h>
#include <MPU6050.h>
#include <DFRobot_L218.h>

MPU6050       mpu;
DFRobot_L218  l218;

void charge()
{  
    if( digitalRead(Charge) == LOW ){
        tone(4,4000,500);
    }
}

void setup() 
{
    SerialUSB.begin(115200);
    l218.init();                                                  //Initialization
    l218.startMPU6050();                                          //Start MPU 6050
    SerialUSB.println("Initialize MPU6050");
    while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){   //Init MPU6050
        SerialUSB.println("Could not find a valid MPU6050 sensor, check wiring!");
        delay(500);
    }

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(Charge), charge, CHANGE);
}

void loop()
{
    Vector normAccel = mpu.readNormalizeAccel();
    int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
    int roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;
    SerialUSB.print(" Pitch = ");
    SerialUSB.print(pitch);
    SerialUSB.print(" Roll = ");
    SerialUSB.print(roll);
    SerialUSB.println();
    delay(10);
}
