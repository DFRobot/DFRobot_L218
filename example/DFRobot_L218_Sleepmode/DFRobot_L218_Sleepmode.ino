 /*
  * File  : DFRobot_L218_Sleepmode.ino
  * Power : L218 powered by 3.7V lithium battery
  * Brief : This example make L218 into sleep mode and wake up
  *         With initialization completed, we can enter AT command to L218 directly
  *         Press button enter the sleep mode or wake up L218 
  *         AT command is invalid after entering sleep mode
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
int  s_mode = 1;

#define  BUTTON    3
#define  CHARGE    6
#define  DONE      7

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
    l218.init();                               //Initialization
    SerialUSB.println("Turn ON L218");
    if(l218.turnON()){                         //Turn ON L218
        SerialUSB.println("Turn ON !");
    }else{
        return;
    }

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE) , charge , CHANGE);
}

void loop(){
    while(SerialUSB.available()){
        Serial1.write(SerialUSB.read());
    }
    SerialUSB.flush();
    while(Serial1.available()){
        SerialUSB.write(Serial1.read());
    }
    if(!digitalRead(BUTTON)){
        delay(50);
        if(!digitalRead(BUTTON)){
            if(s_mode){
                l218.sleepMode();              //L218 enter sleep mode
                SerialUSB.println("Enter sleep mode");
                s_mode = 0;
                while(!digitalRead(BUTTON));
            }else{
                l218.wakeUp();                 //Wake up L218
                SerialUSB.println("Wake UP!");
                s_mode = 1;
                while(!digitalRead(BUTTON));
            }
        }
    }
}