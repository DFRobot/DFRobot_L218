 /*
  * File  : DFRobot_L218_ATtest.ino
  * Power : L218 needs 3.3V DC power supply
  * Brief : This example use the serial port to send AT command to control the L218
  *         Press the button when the net light blinks L218 start
  *         With initialization completed, we can enter AT command to L218 directly
  *  Common AT commands : 
  *         AT+CPIN? : Check SIM card
  *         AT+CSQ   : Check signal quality
  *         AT+CGATT?: Check net attached state
  *         AT+CSTT  : Start net connect task
  *         AT+CIFSR : Get local IP
  *         Get the AT command table in Resource folder
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
  //Initialization
    l218.init();

  //L218 boot interrupt. Press the button for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the button until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(Button) , turn_on,  CHANGE);

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(Charge) , charge ,  CHANGE);

  //Ring interrupt. When there is a phone call, Buzzer sounds. Enter "ATA" for answer the call "ATH" for hang up the call
    attachInterrupt(digitalPinToInterrupt(Ring)   , ring    , CHANGE);
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