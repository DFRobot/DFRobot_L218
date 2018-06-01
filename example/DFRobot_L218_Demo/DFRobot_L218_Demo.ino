 /*
  * File   : DFRobot_Demo.ino
  * Power  : L218 powered by 3.7V lithium battery
  * Brief  : This example use L218 get position temperature and time then send the data to Net whlie MQTT
  *          And store the data in SD card
  * Note   : The tracker function only available in outdoor
  */

#include <MPU6050.h>
#include <DFRobot_L218.h>
#include <avr/dtostrf.h>
#include <SPI.h>
#include <SD.h>
#include <RTCZero.h>

RTCZero          rtc;
MPU6050          mpu;
DFRobot_L218     l218;
File             myFile;
const int        chipSelect = 11;

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

// Change these values to set the current initial time
const byte seconds = 0;
const byte minutes = 0;
const byte hours   = 0;

// Change these values to set the current initial date
const byte day   = 1;
const byte month = 1;
const byte year  = 18;

int   t1=0,t2=0;

void turn_on()
{
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
        digitalWrite(POWER_PIN,LOW );
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

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE_PIN) , charge  , CHANGE);
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
        float temp = mpu.readTemperature();
        SerialUSB.print("Temperature = ");
        delay(500);
        mpu.disableMPU6050();                                   //Disable MPU6050
        char  L218buffer[90] = {0};
        char    latitude[30] = {0};
        char   longitude[30] = {0};
        char temperature[15] = {0};
        char   time_year[ 3] = {0};
        char  time_month[ 3] = {0};
        char    time_day[ 3] = {0};
        char      time_h[ 3] = {0};
        char      time_m[ 3] = {0};
        char      time_s[ 3] = {0};
        itoa(rtc.getYear()    ,time_year  ,10);
        itoa(rtc.getMonth()   ,time_month ,10);
        itoa(rtc.getDay()     ,time_day   ,10);
        itoa(rtc.getHours()   ,time_h     ,10);
        itoa(rtc.getMinutes() ,time_m     ,10);
        itoa(rtc.getSeconds() ,time_s     ,10);
        dtostrf(temp      , 4 , 2 , temperature);
        dtostrf(Longitude , 8 , 5 , longitude  );
        dtostrf(Latitude  , 7 , 5 , latitude   );
        memcpy(L218buffer,"Longitude : ",12);
        strcat(L218buffer, longitude);
        strcat(L218buffer," Latitude : ");
        strcat(L218buffer,latitude);
        strcat(L218buffer," Temperature: ");
        strcat(L218buffer,temperature);
        strcat(L218buffer," Date :");
        strcat(L218buffer,time_year);
        strcat(L218buffer,"/");
        strcat(L218buffer,time_month);
        strcat(L218buffer,"/");
        strcat(L218buffer,time_day);
        strcat(L218buffer," Time :");
        strcat(L218buffer,time_h);
        strcat(L218buffer,":");
        strcat(L218buffer,time_m);
        strcat(L218buffer,":");
        strcat(L218buffer,time_s);
        delay(500);
        SerialUSB.println(L218buffer);
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
        if(l218.MQTTsend(IOT_TOPIC,L218buffer)){                //MQTT send data
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
            myFile.println(L218buffer);                         //Write data to file
            myFile.close();                                     //Save data and close file
            SerialUSB.println("done.");
        }else{
            SerialUSB.println("error opening test.txt");
        }
    }else{
        SerialUSB.println("Please Turn ON L218");
        delay(3000);
    }
}
