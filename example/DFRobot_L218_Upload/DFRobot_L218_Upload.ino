 /*
  * File   : DFRobot_Upload.ino
  * Power  : L218 powered by 3.7V lithium battery
  * Brief  : This example use L218 to get position, temperature and time then upload the data to iot with MQTT
  *          And store the data in SD card. Then device enter sleep mode for 20 seconds
  *          It will do those things again when it wake up
  * Note   : The tracker function only available in outdoor
  */

#include <MPU6050.h>
#include <DFRobot_L218.h>
#include <avr/dtostrf.h>
#include <SPI.h>
#include <SD.h>
#include <RTCZero.h>
#include <ArduinoLowPower.h>
#include <string.h>

RTCZero          rtc;
MPU6050          mpu;
DFRobot_L218     l218;
File             myFile;

#define  BUTTON_PIN    3
#define  CHARGE_PIN    6
#define  DONE_PIN      7
#define  POWER_PIN     9

//Login website to register an account ,fill the following information based on your account
#define serverIP        "iot.dfrobot.com.cn"
#define IOT_CLIENT      " CLIENT NAME "
#define IOT_USERNAME    " USER   NAME "
#define IOT_KEY         " PASSWORD    "
#define IOT_TOPIC       " TOPIC       "

//SD card chip select
const int chipSelect = 11;

// Change these values to set the current initial time
const byte seconds = 0;
const byte minutes = 0;
const byte hours   = 0;

// Change these values to set the current initial date
const byte day   = 1;
const byte month = 1;
const byte year  = 18;

void turn_on()
{
    static int   t1=0,t2=0;
    t1=t2;
    t2=millis();
    if(t1-t2){
        if( digitalRead(BUTTON_PIN) == LOW ){
            tone(4,2000);
            digitalWrite(POWER_PIN,HIGH);
        }else{
            noTone(4);
            digitalWrite(POWER_PIN,LOW );
        }
    }else{
        noTone(4);
    }
}

void charge()
{
    if(digitalRead(DONE_PIN) ){
        if( digitalRead(CHARGE_PIN) == LOW ){
            tone(4,4000,500);
        }
    }
}

void wakeup()
{
    l218.wakeUp();                                              //Wake up L218
}

void setup(){
    SerialUSB.begin(115200);
    while(!SerialUSB);
    l218.init();                                                //Initialization
    rtc.begin();                                                // initialize RTC

    // Set the time
    rtc.setHours(hours);
    rtc.setMinutes(minutes);
    rtc.setSeconds(seconds);

    // Set the date
    rtc.setDay(day);
    rtc.setMonth(month);
    rtc.setYear(year);

    SerialUSB.print("Initializing SD card...");
    if(!SD.begin(chipSelect)){                                  //Init SD card
        SerialUSB.println("initialization failed!");
        return;
    }
    SerialUSB.println("initialization done.");
  //L218 boot interrupt. Press the button for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the button until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN) , turn_on , CHANGE);

  //Battery charge interrupt. When battery get charge from USB, buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE_PIN) , charge  , CHANGE);

  //Wake up interrupt.
    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, wakeup, CHANGE );
}

void loop(){
    if(l218.checkTurnON()){                                     //Check if L218 start
        double  Longitude,Latitude;
        SerialUSB.println("Turn ON !");
        delay(500);
        if(l218.initPos()){                                     //Init positioning functions
            SerialUSB.println("Init position");
            delay(500);
        }else{
            SerialUSB.println("Not init position");
            return;
        }
        while(1){
            delay(500);
            if(l218.getPos()){                                  //Get location information
                SerialUSB.println("Get position");
                Longitude = l218.getLongitude();                //Get longitude
                Latitude  = l218.getLatitude();                 //Get latitude
                SerialUSB.print("Longitude = ");
                SerialUSB.print(Longitude,6);
                SerialUSB.print("Latitude  = ");
                SerialUSB.print(Latitude ,6);
                break;
            }else{
                SerialUSB.println("Not position");
            }               
        }
        mpu.enableMPU6050();                                    //Enable MPU6050
        SerialUSB.println("Initialize MPU6050");
        //Init MPU6050
        while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
            SerialUSB.println("Could not find a valid MPU6050 sensor, check wiring!");
            delay(500);
        }
        float temperature = mpu.readTemperature();
        delay(500);
        mpu.disableMPU6050();                                   //Disable MPU6050
        String  l218Buffer;
        l218Buffer  = String();
        l218Buffer += "Longitude : ";
        l218Buffer +=  Longitude;
        l218Buffer +=" Latitude : ";
        l218Buffer +=  Latitude;
        l218Buffer += " Temperature: ";
        l218Buffer +=  temperature;
        l218Buffer += " Date :";
        l218Buffer += rtc.getYear();
        l218Buffer += "/"; 
        l218Buffer += rtc.getMonth();
        l218Buffer += "/";
        l218Buffer += rtc.getDay();
        l218Buffer += " Time :";
        l218Buffer += rtc.getHours();
        l218Buffer += ":";
        l218Buffer += rtc.getMinutes();
        l218Buffer += ":";
        l218Buffer += rtc.getSeconds();
        SerialUSB.println(l218Buffer);
        if(l218.checkSIMcard()){                                //Check SIM card
            SerialUSB.println("Card ready");
            delay(500);
        }else{
            SerialUSB.println("NO Card");
            return;
        }
        if(l218.initNetwork()){                                 //Init network functions
            SerialUSB.println("NET ONLINE!");
            delay(2000);
        }else{
            SerialUSB.println("NO NET");
            return;
        }
        if(l218.connect(serverIP,TCP,1883)){                    //Connect to server
            SerialUSB.println("connected");
        }else{
            SerialUSB.println("disconnect");
            return;
        }
        if(l218.MQTTconnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){  //MQTT connect requst
            SerialUSB.println("MQTT connected");
            delay(2000);
        }else{
            SerialUSB.println("MQTT connect failed");
            return;
        }
        if(l218.MQTTsend(IOT_TOPIC,l218Buffer)){                //MQTT send data
            SerialUSB.println("Send OK");
        }else{
            SerialUSB.println("MQTT fail to send");
            return;
        }
        if(l218.MQTTdisconnect()){                              //MQTT disconnect requst
            SerialUSB.println("MQTT disconnected");
        }else{
            SerialUSB.println("Fail to disconnect");
        }
        if(l218.disconnect()){                                  //Disconnect from server
            SerialUSB.println("Server disconnected");
        }else{
            SerialUSB.println("Fail to disconnect");
        }
        myFile = SD.open("data.txt", FILE_WRITE);               //Open SD card file in write mode
        if(myFile){
            SerialUSB.print("Writing to test.txt...");
            myFile.println(l218Buffer);                         //Write data to file
            myFile.close();                                     //Save data and close file
            SerialUSB.println("done.");
        }else{
            SerialUSB.println("error opening test.txt");
        }
        l218.sleepMode();                                       //L218 enter sleep mode
        LowPower.sleep(20000);                                  //Processor enter sleep mode for 20000ms
    }else{
        SerialUSB.println("Please Turn ON L218");
        delay(3000);
    }
}
