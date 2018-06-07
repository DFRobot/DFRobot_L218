 /*
  * File  : DFRobot_L218_Sleepmode.ino
  * Power : L218 powered by 3.7V lithium battery
  * Brief : This example make L218 into sleep mode and wake up
  *         With initialization completed, the LED blink once then device enter the sleep mode
  *         The internal RTC will wake up the device every 20 seconds
  *         The number of LED blinks increased one after wake up
  * Note  : Please make sure you have import Arduino Low Power library
  *         To use a library in a sketch, select it from Sketch > Import Library. 
  */

#include <DFRobot_L218.h>
#include <ArduinoLowPower.h>

DFRobot_L218  l218;
int  repetitions = 1;

#define  BUTTON_PIN    3
#define  CHARGE_PIN    6
#define  DONE_PIN      7
#define  LED_PIN      13

void charge()
{
    if(digitalRead(DONE_PIN)){
        if( digitalRead(CHARGE_PIN) == LOW ){
            tone(4,4000,500);
        }
    }
}

void wakeup()
{
    l218.wakeUp();                             //Wake up L218
    tone(4,3000,500);
    repetitions ++;
}

void setup(){
    SerialUSB.begin(115200);
    l218.init();                               //Initialization
    SerialUSB.println("Turn ON L218");
    if(l218.turnON()){                         //Turn ON L218
        SerialUSB.println("Turn ON !");
    }else{
        return;
    }

  //Battery charge interrupt. When battery get charge from USB, buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE_PIN) , charge , CHANGE);
  //Wake up interrupt. Press the button to wake up device
    LowPower.attachInterruptWakeup(BUTTON_PIN      , wakeup, FALLING);
  //RTC wake up interrupt.
    LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, wakeup, CHANGE );
}

void loop(){
    for(int i = 0; i < repetitions; i++){
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
    l218.sleepMode();                          //L218 enter sleep mode
    LowPower.sleep(20000);                     //Processor enter sleep mode for 20s
}