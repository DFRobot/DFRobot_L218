 /*
  * File   : DFRobot_L218_Tracker.ino
  * Power  : L218 powered by 3.7V lithium battery
  * Brief  : This example use L218 as a tracker
  *          Press the button when the net light blinks L218 start
  *          After initialization is completed it will print longitude (Positive numbers for East and Negative numbers for West)
  *          and latitude (Positive numbers for North and Negative numbers for South)
  *          Thus we finished the positioning function verification
  * Note   : The tracker function only available in outdoor
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

#define  BUTTON    3
#define  CHARGE    6
#define  DONE      7
#define  POWER     9

void turn_on()
{  
    if( digitalRead(BUTTON) == LOW ){
        tone(4,2000);
        digitalWrite(POWER,HIGH);
    }else{
        noTone(4);
        digitalWrite(POWER,LOW);
    }
}

void charge()
{
    if(digitalRead(DONE)){
        if( digitalRead(CHARGE) == LOW ){
            tone(4,4000,500);
        }
    }
}

void setup(){
    SerialUSB.begin(115200);
    while(!SerialUSB);
    l218.init();                                          //Initialization

  //L218 boot interrupt. Press the button for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the button until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(BUTTON) , turn_on , CHANGE);

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE) , charge  , CHANGE);
}

void loop(){
    if(l218.checkTurnON()){                               //Check if L218 start
        delay(10000);
        SerialUSB.println("Turn ON !");
        delay(500);
        if(l218.initPos()){                               //Init positioning functions
            SerialUSB.println("Init position");
            delay(500);
        }else{
            SerialUSB.println("Not init position");
            return;
        }
        while(1){
            delay(2000);
            if(l218.getPos()){                            //Get location information
                SerialUSB.println("Get position");
                double    Longitude,Latitude;
                Longitude = l218.getLongitude();          //Get longitude
                Latitude  = l218.getLatitude();           //Get latitude
                SerialUSB.print("Longitude = ");
                SerialUSB.print(Longitude,6);
                SerialUSB.print("Latitude  = ");
                SerialUSB.print(Latitude ,6);
            }else{
                SerialUSB.println("Not position");
            }
        }
    }
}

int readSerial(char result[]){
    int i = 0;
    while(1){
        while(SerialUSB.available() > 0){
            char inChar = SerialUSB.read();
            if(inChar == '\n'){
                result[i] = '\0';
                SerialUSB.flush();
                return 0;
            }
            if(inChar != '\r'){
                result[i] = inChar;
                i++;
            }
        }
    }
}