 /*
  * File  : DFRobot_L218_ATtest.ino
  * Power : L218 needs 3.3V DC power supply
  * Brief : This example use the serial port to send AT command to control the L218
  *         Press the button when the net light blinks L218 start
  *         With initialization completed, we can enter AT command to L218 directly
  * AT command list:
  *
  * Note  : The AT command must end with CRLF
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

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
    if( digitalRead(Charge) == LOW ){
        tone(4,4000,500);
    }
}

void ring()
{
    if( digitalRead(Ring) == LOW ){
        tone(4,8000);
        SerialUSB.println("Ring ! ! !");
    }else{
        noTone(4);
        SerialUSB.println("Ring Over");
    }
}

void setup(){
    SerialUSB.begin(115200);
    l218.init();
    attachInterrupt(digitalPinToInterrupt(Button) , turn_on,  CHANGE);  //L218 boot interrupt
    attachInterrupt(digitalPinToInterrupt(Charge) , charge ,  CHANGE);  //Battery charge interrupt
    attachInterrupt(digitalPinToInterrupt(Ring)   , ring    , CHANGE);  //Ring interrupt
}

void loop(){
    while(SerialUSB.available()){
        Serial1.write(SerialUSB.read());
    }
    SerialUSB.flush();
    while(Serial1.available()){
        SerialUSB.write(Serial1.read());
    }
}