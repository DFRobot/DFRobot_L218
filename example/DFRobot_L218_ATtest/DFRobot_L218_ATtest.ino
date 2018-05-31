 /*
  * File  : DFRobot_L218_ATtest.ino
  * Power : L218 powered by 3.7V lithium battery
  * Brief : This example use the serial port to send AT command to control the L218
  *         Press the BUTTON_PIN when the net light blinks L218 start
  *         With initialization completed, we can enter AT command to L218 directly
  *  Common AT commands : 
  *         AT+CPIN? : Check SIM card
  *         AT+CSQ   : Check signal quality
  *         AT+CGATT?: Check net attached state
  *         AT+CSTT  : Start net connect task
  *         AT+CIFSR : Get local IP
  *         Get the AT command table in Resource folder :GSM_GNSS_AT_DOCUMENT_V2.8
  * Note  : The AT command must end with CRLF
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

#define  BUTTON_PIN    3
#define  CHARGE_PIN    6
#define  DONE_PIN      7
#define  RING      8
#define  POWER_PIN     9

void turn_on()
{  
    if( digitalRead(BUTTON_PIN) == LOW ){
        tone(4,2000);
        digitalWrite(POWER_PIN,HIGH);
    }else{
        noTone(4);
        digitalWrite(POWER_PIN,LOW );
    }
}

void charge()
{
    if(digitalRead(DONE_PIN)){
        if( digitalRead(CHARGE_PIN) == LOW ){
            tone(4,4000,500);
        }
    }
}

void ring()
{
    if( digitalRead(RING) == LOW ){
        tone(4,8000);
        SerialUSB.println("Ring ! ! !");
    }else{
        noTone(4);
        SerialUSB.println("Ring Over");
    }
}

void setup(){
    SerialUSB.begin(115200);
    while(!SerialUSB);
  //Initialization
    l218.init();

  //L218 boot interrupt. Press the BUTTON_PIN for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the BUTTON_PIN until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN) , turn_on,  CHANGE);

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE_PIN) , charge ,  CHANGE);

  //Check if L218 start
    while(!l218.checkTurnON()){
        SerialUSB.println("Please Turn ON L218");
        delay(3000);
    }
  //Ring interrupt. When there is a phone call, Buzzer sounds. Enter "ATA" for answer the call "ATH" for hang up the call
    attachInterrupt(digitalPinToInterrupt(RING)   , ring    , CHANGE);
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