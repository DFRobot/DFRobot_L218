 /*
  * File  : DFRobot_MPU6050_Temperature.ino
  * Brief : This is MPU6050 Triple Axis Gyroscope & Accelerometer. 
  *         Temperature Example.
  */


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
    l218.init();
    l218.startMPU6050();
    SerialUSB.println("Initialize MPU6050");
    while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
        SerialUSB.println("Could not find a valid MPU6050 sensor, check wiring!");
        delay(500);
    }
    attachInterrupt(digitalPinToInterrupt(Charge) , charge , CHANGE);
}

void loop()
{
    float temp = mpu.readTemperature();
    SerialUSB.print(" Temp = ");
    SerialUSB.print(temp);
    SerialUSB.println(" *C");
    delay(500);
}
