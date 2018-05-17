 /*
  * File  : DFRobot_l218_sendShortMessage.ino
  * Brief : This example use for send short message
  *         Press the button when the net light blinks L218 start
  *         After initialization is completed ennter a phone number then enter the message data
  *         Thus we finished the Short Message verification
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

char phoneNum[20];
char message[200];

void turn_on()
{
    if( digitalRead(Button) == LOW ){
        tone(4,2000);
        digitalWrite(power,HIGH);
    }else{
        noTone(4);
        digitalWrite(power,LOW);
    }
}

void charge()
{
    if(digitalRead(Done)){
        if( digitalRead(Charge) == LOW ){
            tone(4,4000,500);
        }
    }
}

void ring()
{
    if( digitalRead(Ring) == LOW ){
        tone(4,4000);
        SerialUSB.println("Ring ! ! !");
    }else{
        noTone(4);
        SerialUSB.println("Ring Over");
    }
}


void setup(){
    SerialUSB.begin(115200);
    l218.init();
    attachInterrupt(digitalPinToInterrupt(Button) , turn_on , CHANGE);     //L218 boot interrupt
    attachInterrupt(digitalPinToInterrupt(Charge) , charge  , CHANGE);     //Battery charge interrupt
}

void loop(){
    delay(5000);
    if(l218.check_TurnON()){                                               //Check if L218 start
        SerialUSB.println("Turn ON !");
        attachInterrupt( digitalPinToInterrupt(Ring)  , ring   , CHANGE);  //Ring interrupt
        if(l218.check_SIMcard()){                                          //Check SIM card
              SerialUSB.println("Card ready");
              delay(2000);
              SerialUSB.println("Input phone number:");
              readSerial(phoneNum);
        }else{
            SerialUSB.println("NO Card");
            return;
        }
        if(l218.beginSMS(phoneNum)){                                       //Get phone number
            SerialUSB.println("OK");
            SerialUSB.println("Enter the message content: ");
            readSerial(message);
        }else{
            SerialUSB.println("Fail");
            return;
        }
        if(l218.sendSMS(message)){                                         //Get message
            SerialUSB.println("Send OK");
        }else{
            SerialUSB.println("Fail to send");
            return;
        }
    }
    delay(5000); 

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