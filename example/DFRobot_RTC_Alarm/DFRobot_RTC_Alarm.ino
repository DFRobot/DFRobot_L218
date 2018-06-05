 /*
  * File  : DFRobot_RTC_Alarm.ino
  * Brief : This example is a RTC alarm clock
  *         Set the current initial date and time
  *         Set alarm clock ,when buzzer ring press button to stop alarm
  * Note  : Plesae make sure you have import Arduino RTC Zero library
  *         To use a library in a sketch, select it from Sketch > Import Library. 
  */

#include <RTCZero.h>

RTCZero rtc;

#define  BUTTON_PIN    3

// Change these values to set the current initial time
const byte seconds = 0;
const byte minutes = 0;
const byte hours   = 0;

// Change these values to set the current initial date
const byte day     = 1;
const byte month   = 1;
const byte year    = 18;

void alarmMatch()
{
    tone(4,2000);
}

void stop()
{
    noTone(4);
}

void setup()
{
    SerialUSB.begin(115200);
    while(!SerialUSB);
    rtc.begin();           // initialize RTC 24H format
    rtc.setTime(hours, minutes, seconds);
    rtc.setDate(day  , month  , year   );

  //Set alarm hours, minutes, seconds
    rtc.setAlarmTime( 0, 0, 10);
  //Enable alarm
    rtc.enableAlarm(rtc.MATCH_HHMMSS);
    rtc.attachInterrupt(alarmMatch);
  //Stop alarm interrupt. Press the button stop alarm
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN) , stop , CHANGE);
}

void loop()
{
    
}
