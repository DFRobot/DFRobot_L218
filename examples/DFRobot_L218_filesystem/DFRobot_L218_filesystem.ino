 /*
  * file DFRobot_L218_filesystem.ino
  * brief DFRobot's L218 module
  * This example use for L218 file system
  * After initialization is completed it will list all files and create folder, create file in new folder, write data to new file
  * Read data from new file then delete new file and folder.It will show ever step result.
  */

#include <Wire.h>
#include <DFRobot_L218.h>

#define PIN_TX     7
#define PIN_RX     8
SoftwareSerial     mySerial(PIN_RX,PIN_TX);
DFRobot_L218       l218;

void setup(){
    Serial.begin(115200);
    delay(2000);
    l218.begin(mySerial);                                         //Set SoftwareSerial
    Serial.println("L218 file system");
    Serial.println("Init file system......");
    bool Connected = false;
    while(!Connected){
        if(l218.initFile()){                                      //Init file system
            Connected = true;
        }else{
            Serial.println("Fail to init file syste");
            delay(1000);
        }
    }
    Serial.println("File system initialized");

}

void loop(){
    Serial.println("Space info:");
    Serial.print(l218.getSpace());
    Serial.println("List file :");
    Serial.print(l218.getList());                                 //Get the list of all files
    delay(50);
    Serial.println("Create folder :");
    if(l218.createFolder("test")){                                //Create a folder
        Serial.println("New file list :");
        Serial.print(l218.getList());
        delay(50);
    }else{
        Serial.println("Fail to create folder");
        while(1);
    }
    Serial.println("New folder list :");
    Serial.print(l218.getList("test"));                           //Get the list of specified folder
    delay(50);
    Serial.println("Create file :");
    if(l218.createFile("test/test.txt")){                         //Create a file
        Serial.println("New folder list :");
        Serial.print(l218.getList("test"));
        delay(50);
    }else{
        Serial.println("Fail to create file");
        while(1);
    }
    if(l218.writeFile("test/test.txt", "Hi DFRobot")){            //Write data to specified file
        Serial.println("Read file :");
        Serial.print(l218.readFile("test/test.txt",0,10));        //Read data from specified file
        Serial.println("Read file in hex mode :");
        Serial.print(l218.readFile("test/test.txt",0,10,Hex));    //Read data from specified file in hex mode
         delay(50);
    }else{
        Serial.println("Fail to write file");
        while(1);
    }
    Serial.println("Space info:");
    Serial.print(l218.getSpace());
    Serial.println("Delete file");
    if(l218.deleteFile("test/test.txt")){                         //Delete the file
        Serial.println("New folder list :");
        Serial.println("test");
        delay(50);
    }else{
        Serial.println("Fail to delet file");
        while(1);
    }
    Serial.println("Delete folder");
    if(l218.deleteFolder("test")){                                //Delete the folder
        Serial.println("New file list :");
        Serial.print(l218.getList());
        Serial.println("Space info:");
        Serial.print(l218.getSpace());
    }else{
        Serial.println("Fail to delet folder");
    }
    while(1);
}
