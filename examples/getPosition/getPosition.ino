 /*
  * file getPostion.ino
  * brief DFRobot's L218 module
  * This example use for get time ,longitude and latitude
  * After initialization is completed u will get time and enter anything to get longitude and latitude
  */

#include <Wire.h>
#include <DFRobot_L218.h>

#define PIN_TX     7
#define PIN_RX     8
SoftwareSerial     mySerial(PIN_RX,PIN_TX);
DFRobot_L218       l218;

void setup(){
    Serial.begin(115200);
    Serial.println("Get Postion");
    Serial.println("Check and init SIMcard......");
    bool Connected = false;
    l218.begin(mySerial);                                       //Set SoftwareSerial
    while(!Connected){
        if(l218.init()){                                        //Check and init SIMcard
            Connected = true;
        }else{
            Serial.println("Not connected");
        }
    }
    Serial.println("SIM initialized.");
    Serial.println("Init positioning function......");
    Connected = false;
    while(!Connected){
        if(l218.initPos()){                                     //Init GPS module
            Connected = true;
        }else{
            Serial.println("Fail to init positioning function");
            delay(1000);
        }
    }
    Serial.println("Positioning function initialized");
}

void loop(){
    Serial.println("Enter anything to get positioning ");
    char loge[10];
    readSerial(loge);
    if(l218.getPosition()){                                     //Get the current position
        Serial.print("Longtude :");
        Serial.println(l218.getLongitude());                    //Get longitude
        Serial.print("Latitude :");
        Serial.println(l218.getLatitude());                     //Get latitude
    }else{
        Serial.println("Wrong data try again");
    }
}

int readSerial(char result[]){
    int i = 0;
    while(1){
        while(Serial.available() > 0){
            char inChar = Serial.read();
            if(inChar == '\n'){
                result[i] = '\0';
                Serial.flush();
                return 0;
            }
            if(inChar != '\r'){
                result[i] = inChar;
                i++;
            }
        }
    }
}