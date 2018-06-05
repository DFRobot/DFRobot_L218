 /*
  * File  : DFRobot_RTC_GetTime.ino
  * Brief : This example is a RTC clock
  *         Set the current initial date and time, it works like a clock
  * Note  : Plesae make sure you have import Arduino RTC Zero library
  *         To use a library in a sketch, select it from Sketch > Import Library. 
  */

#include <RTCZero.h>

RTCZero rtc;

// Change these values to set the current initial time
const byte seconds = 0;
const byte minutes = 0;
const byte hours   = 0;

// Change these values to set the current initial date
const byte day   = 1;
const byte month = 1;
const byte year  = 18;

void setup()
{
    SerialUSB.begin(115200);
    rtc.begin();                // initialize RTC

    // Set the time
    rtc.setHours(hours);
    rtc.setMinutes(minutes);
    rtc.setSeconds(seconds);

    // Set the date
    rtc.setDay(day);
    rtc.setMonth(month);
    rtc.setYear(year);
}

void loop()
{
    // Print date...
    print2digits(rtc.getDay());
    SerialUSB.print("/");
    print2digits(rtc.getMonth());
    SerialUSB.print("/");
    print2digits(rtc.getYear());
    SerialUSB.print(" ");

    // ...and time
    print2digits(rtc.getHours());
    SerialUSB.print(":");
    print2digits(rtc.getMinutes());
    SerialUSB.print(":");
    print2digits(rtc.getSeconds());
    SerialUSB.println();
    delay(1000);
}

void print2digits(int number) {
    if(number < 10){
        SerialUSB.print("0");   // print a 0 before if the number is < than 10
    }
    SerialUSB.print(number);
}