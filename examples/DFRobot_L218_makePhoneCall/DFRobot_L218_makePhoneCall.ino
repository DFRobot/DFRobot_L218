 /*
  * File  : DFRobot_L218_makePhoneCall.ino
  * Brief : DFRobot's SIM module
  *         This example use for make a phone call
  *         After initialization is completed enter a phone number to make a phone call then enter anything to hang call
  */

#include <Wire.h>
#include <DFRobot_L218.h>

#define PIN_TX     7
#define PIN_RX     8
SoftwareSerial     mySerial(PIN_RX,PIN_TX);
DFRobot_L218       l218;

String remoteNumber = "";                                               //The number you will call
char charbuffer[20];

void setup(){
    Serial.begin(115200);
    l218.begin(mySerial);                                               //Set SoftwareSerial
    Serial.println("Make Voice Call");
    Serial.println("Check and init SIMcard......");
    bool Connected = false;
    while(!Connected){
        if(l218.init()){                                                //Check and init SIMcard
            Connected = true;
        }else{
            Serial.println("Not connected");
        }
    }
    Serial.println("SIM initialized.");
    Serial.println("Enter phone number to call.");
}

void loop() {
    while(Serial.available() > 0){
        char inChar = Serial.read();
        if(inChar == '\n'){
            if(remoteNumber.length() < 25){
                Serial.print("Calling to : ");
                Serial.println(remoteNumber);
                remoteNumber.toCharArray(charbuffer, 25);
                if(l218.voiceCall(charbuffer)){                         //Make a phonecall
                    Serial.println("Call Established. Enter line to end");
                    while(Serial.read() != '\n');
                    Serial.println("Hang call");
                    l218.hangCall();                                    //Hang call
                }
                Serial.println("Call Finished");
                remoteNumber = "";
                Serial.println("Enter phone number to call.");
            }else{
                Serial.println("That's too long for a phone number. I'm forgetting it");
                remoteNumber = "";
            }
        }else{
            if (inChar != '\r'){
            remoteNumber += inChar;
            }
        }
    }
}