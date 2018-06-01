 /*
  * File  : DFRobot_L218_sendShortMessage.ino
  * Power : L218 powered by 3.7V lithium battery
  * Brief : This example use for send short message
  *         Press the button when the net light blinks L218 start
  *         After initialization is completed ennter a phone number then enter the message data
  *         Thus we finished the Short Message verification
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

#define  BUTTON_PIN    3
#define  CHARGE_PIN    6
#define  DONE_PIN      7
#define  POWER_PIN     9

char phoneNum[20];
char message[200];
int  t1=0,t2=0;

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
    l218.init();

  //L218 boot interrupt. Press the button for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the button until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN) , turn_on , CHANGE);

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE_PIN) , charge  , CHANGE);
}

void loop(){
    if(l218.checkTurnON   ()){                                  //Check if L218 start
        SerialUSB.println("Turn ON !");
        if(l218.checkSIMcard()){                                //Check SIM card
            SerialUSB.println("Card ready");
            delay(2000);
            SerialUSB.println("Input phone number:");
            readSerial(phoneNum);
        }else{
            SerialUSB.println("NO Card");
            return;
        }
        if(l218.beginSMS(phoneNum)){                            //Get phone number
            SerialUSB.println("OK");
            SerialUSB.println("Enter the message content: ");
            readSerial(message);
        }else{
            SerialUSB.println("Fail");
            return;
        }
        if(l218.sendSMS(message)){                              //Get message
            SerialUSB.println("Send OK");
        }else{
            SerialUSB.println("Fail to send");
            return;
        }
    }else{
        SerialUSB.println("Please Turn ON L218");
        delay(3000);
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