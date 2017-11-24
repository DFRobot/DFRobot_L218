 /*
  * file filesystem.ino
  * brief DFRobot's SIM module
  * This example use for L218 file system
  * After initialization is completed it will list all files and create folder, create file in new folder, write data to new file
  * Read data from new file then delete new file and folder.It will show ever step result.
  */

#include <Wire.h>
#include <DFRobot_SIM.h>
#include <DFRobot_L218.h>

SoftwareSerial   mySerial(8,7);                                       //RX TX
DFSIM            sim;
DFL218File       L218F;

void setup(){
    Serial.begin(115200);
    sim.begin(mySerial);                                                   //Set SoftwareSerial
    Serial.println("L218 file system");
    Serial.println("Init file system......");
    bool Connected = false;
    while(!Connected){
        if(L218F.init()){                                                  //Init file system
            Connected = true;
        }else{
            Serial.println("Fail to init file syste");
            delay(1000);
        }
    }
    Serial.println("File system initialized");

}

void loop(){
    Serial.println("List file :");
    Serial.print(L218F.getList());                                         //Get the list of all files
    delay(50);
    Serial.println("Create folder :");
    if(L218F.createFolder("test")){                                        //Create a folder
        Serial.println("New file list :");
        Serial.print(L218F.getList());
        delay(50);
        Serial.println("New folder list :");
        Serial.print(L218F.getList("test"));                               //Get the list of specified folder
        delay(50);
        Serial.println("Create file :");
        if(L218F.createFile("test/test.txt")){                             //Create a file
            Serial.println("New folder list :");
            Serial.print(L218F.getList("test"));
            delay(50);
            if(L218F.writeFile("test/test.txt", "Hi DFRobot")){            //Write data to specified file
                Serial.println("Read file :");
                Serial.print(L218F.readFile("test/test.txt",0,10));        //Read data from specified file
                Serial.println("Read file in hex mode :");
                Serial.print(L218F.readFile("test/test.txt",0,10,Hex));    //Read data from specified file in hex mode
                delay(50);
                Serial.println("Delete file");
                if(L218F.deleteFile("test/test.txt")){                     //Delete the file
                    Serial.println("New folder list :");
                    Serial.println("test");
                    delay(50);
                    Serial.println("Delete folder");
                    if(L218F.deleteFolder("test")){                        //Delete the folder
                        Serial.println("New file list :");
                        Serial.print(L218F.getList());
                    }else{
                        Serial.println("Fail to delet folder");
                    }
                }else{
                    Serial.println("Fail to delet file");
                }
            }else{
                Serial.println("Fail to write file");
            }
        }else{
            Serial.println("Fail to create file");
        }
    }else{
        Serial.println("Fail to create folder");
    }
    while(1);
}
