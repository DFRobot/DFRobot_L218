 /*
  * File  : DFRobot_MPU6050_Free_fall.ino
  * Brief : This is MPU6050 Triple Axis Gyroscope & Accelerometer. 
  *         Free fall detection.
  */

#include <MPU6050.h>
#include <DFRobot_L218.h>

MPU6050       mpu;
DFRobot_L218  l218;

boolean ledState           = false;
boolean freefallDetected   = false;
int     freefallBlinkCount = 0;

void charge()
{  
    if( digitalRead(Charge) == LOW ){
        tone(4,4000,500);
    }
}

void setup() 
{
    SerialUSB.begin(115200);
    while(!SerialUSB);
    l218.init();
    l218.startMPU6050();
    attachInterrupt(digitalPinToInterrupt(Charge) , charge , CHANGE);
    SerialUSB.println("Initialize MPU6050");
    while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G)){
        SerialUSB.println("Could not find a valid MPU6050 sensor, check wiring!");
        delay(500);
    }
    mpu.setAccelPowerOnDelay(MPU6050_DELAY_3MS);
    mpu.setIntFreeFallEnabled(true);
    mpu.setIntZeroMotionEnabled(false);
    mpu.setIntMotionEnabled(false);
    mpu.setDHPFMode(MPU6050_DHPF_5HZ);
    mpu.setFreeFallDetectionThreshold(17);
    mpu.setFreeFallDetectionDuration(2);
    checkSettings();
    pinMode(4, OUTPUT);
    digitalWrite(4, LOW);
    attachInterrupt(0, doInt, RISING);
}

void doInt()
{
    freefallBlinkCount = 0;
    freefallDetected = true;  
}

void checkSettings()
{
    SerialUSB.println();
    SerialUSB.print(" * Sleep Mode:                ");
    SerialUSB.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
    SerialUSB.print(" * Motion Interrupt:     ");
    SerialUSB.println(mpu.getIntMotionEnabled() ? "Enabled" : "Disabled");
    SerialUSB.print(" * Zero Motion Interrupt:     ");
    SerialUSB.println(mpu.getIntZeroMotionEnabled() ? "Enabled" : "Disabled");
    SerialUSB.print(" * Free Fall Interrupt:       ");
    SerialUSB.println(mpu.getIntFreeFallEnabled() ? "Enabled" : "Disabled");
    SerialUSB.print(" * Free Fal Threshold:          ");
    SerialUSB.println(mpu.getFreeFallDetectionThreshold());
    SerialUSB.print(" * Free FallDuration:           ");
    SerialUSB.println(mpu.getFreeFallDetectionDuration());
    SerialUSB.print(" * Clock Source:              ");
    switch(mpu.getClockSource()){
        case MPU6050_CLOCK_KEEP_RESET:
            SerialUSB.println("Stops the clock and keeps the timing generator in reset");
            break;
        case MPU6050_CLOCK_EXTERNAL_19MHZ: 
            SerialUSB.println("PLL with external 19.2MHz reference");
            break;
        case MPU6050_CLOCK_EXTERNAL_32KHZ:
            SerialUSB.println("PLL with external 32.768kHz reference");
            break;
        case MPU6050_CLOCK_PLL_ZGYRO:
            SerialUSB.println("PLL with Z axis gyroscope reference");
            break;
        case MPU6050_CLOCK_PLL_YGYRO:
            SerialUSB.println("PLL with Y axis gyroscope reference");
            break;
        case MPU6050_CLOCK_PLL_XGYRO:
            SerialUSB.println("PLL with X axis gyroscope reference");
            break;
        case MPU6050_CLOCK_INTERNAL_8MHZ:  
            SerialUSB.println("Internal 8MHz oscillator");
            break;
    }
    SerialUSB.print(" * Accelerometer:             ");
    switch(mpu.getRange()){
    case MPU6050_RANGE_16G:
        SerialUSB.println("+/- 16 g"); 
        break;
    case MPU6050_RANGE_8G:
        SerialUSB.println("+/- 8 g");
        break;
    case MPU6050_RANGE_4G:
        SerialUSB.println("+/- 4 g");
        break;
    case MPU6050_RANGE_2G:             
        SerialUSB.println("+/- 2 g");
        break;
    }  
    SerialUSB.print(" * Accelerometer offsets:     ");
    SerialUSB.print(mpu.getAccelOffsetX());
    SerialUSB.print(" / ");
    SerialUSB.print(mpu.getAccelOffsetY());
    SerialUSB.print(" / ");
    SerialUSB.println(mpu.getAccelOffsetZ());
    SerialUSB.print(" * Accelerometer power delay: ");
    switch(mpu.getAccelPowerOnDelay()){
        case MPU6050_DELAY_3MS:
            SerialUSB.println("3ms");
            break;
        case MPU6050_DELAY_2MS:
            SerialUSB.println("2ms");
            break;
        case MPU6050_DELAY_1MS:
            SerialUSB.println("1ms");
            break;
        case MPU6050_NO_DELAY:
            SerialUSB.println("0ms");
            break;
    }
    SerialUSB.println();
}

void loop()
{
    Vector rawAccel = mpu.readRawAccel();
    Activites act = mpu.readActivites();
    SerialUSB.print(act.isFreeFall);
    SerialUSB.print("\n");
    if (freefallDetected){
        ledState = !ledState;
        digitalWrite(13, ledState);
        freefallBlinkCount++;
        if (freefallBlinkCount == 20){
            freefallDetected = false;
            ledState = false;
            digitalWrite(13, ledState);
        }
    }
    delay(100);
}
