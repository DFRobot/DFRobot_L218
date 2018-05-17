 /*
  * File  : DFRobot_SD_ListFile.ino
  * Brief : This example list SD card file
  */

#include <SPI.h>
#include <SD.h>

File root;
const int chipSelect = 11;

void setup() {
    SerialUSB.begin(9600);
    while (!SerialUSB) {
        ;
    }
    SerialUSB.print("Initializing SD card...");
    if (!SD.begin(chipSelect)) {
        SerialUSB.println("initialization failed!");
        return;
    }
    SerialUSB.println("initialization done.");
    root = SD.open("/");
    printDirectory(root, 0);
    SerialUSB.println("done!");
}

void loop() {
    // nothing happens after setup finishes.
}

void printDirectory(File dir, int numTabs) {
    while(true){
        File entry = dir.openNextFile();
        if(! entry){
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            SerialUSB.print('\t');
        }
        SerialUSB.print(entry.name());
        if(entry.isDirectory()){
            SerialUSB.println("/");
            printDirectory(entry, numTabs + 1);
        }else{
            SerialUSB.print("\t\t");
            SerialUSB.println(entry.size(), DEC);
        }
        entry.close();
    }
}

