 /*
  * File  : DFRobot_SD_Readrite.ino
  * Brief : This example open SD card file write data to it and read data from it
  */

#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 11;

void setup() {
    SerialUSB.begin(9600);
    while(!SerialUSB){
        ;
    }
    SerialUSB.print("Initializing SD card...");
    if(!SD.begin(chipSelect)){
        SerialUSB.println("initialization failed!");
        return;
    }
    SerialUSB.println("initialization done.");
    myFile = SD.open("test.txt", FILE_WRITE);
    if(myFile){
        SerialUSB.print("Writing to test.txt...");
        myFile.println("testing 1, 2, 3.");
        myFile.close();
        SerialUSB.println("done.");
    }else{
        SerialUSB.println("error opening test.txt");
    }
    myFile = SD.open("test.txt");
    if(myFile){
        SerialUSB.println("test.txt:");
        while(myFile.available()) {
            SerialUSB.write(myFile.read());
        }
        myFile.close();
    }else{
        SerialUSB.println("error opening test.txt");
    }
}

void loop() {
  // nothing happens after setup
}