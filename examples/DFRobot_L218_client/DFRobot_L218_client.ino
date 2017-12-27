 /*
  * File  : DFRobot_L218_client.ino
  * Brief : This example use for connect to iot and communicate
  *         After initialization is completed, connect to server IP and communicate with IOT as following step
  *         Connect request > Send data >Subscribe topic > Recive data from topic > Unsubscribe > Disconnect
  *         Thus we finished the IOT connection and communication verification
  */

#include <Wire.h>
#include <DFRobot_L218.h>

#define serverIP        "iot.dfrobot.com.cn"
#define IOT_CLIENT      "test"
#define IOT_USERNAME    "HJmwapNfG"
#define IOT_KEY         "B1gmvpT4fM"
#define IOT_TOPIC       "HJEv6TVMM"
#define PIN_TX           7
#define PIN_RX           8
SoftwareSerial           mySerial(PIN_RX,PIN_TX);
DFRobot_L218             l218;

void setup(){
    int signalQuality;
    Serial.begin(115200);
    delay(2000);
    l218.begin(mySerial);                                     //Set SoftwareSerial
    Serial.println("SIM Client");
    Serial.println("Check and init SIMcard......");
    bool Connected = false;
    while(!Connected){
        if(l218.init()){                                      //Check and init SIMcard
            Connected = true;
        }else{
            Serial.println("Not connected");
        }
    }
    Serial.println("SIM initialized.");
    Connected = false;
    while(!Connected){
        signalQuality = l218.checkSignalQuality();            //Check signal quality
        if(signalQuality){
            Serial.print("Signal Quality = ");
            Serial.println(signalQuality);
            Connected = true;
        }else{
            Serial.println("No Signal ! ");
            delay(500);
        }
    }
    Connected = false;

    Serial.println("Init connect server......");
    while(!Connected){
        if(l218.initNet()){                                   //Init net module
            Serial.println("Done !");
            Connected = true;
        }else{
            Serial.println("Failed !");
            delay(500);
        }
    }
}

void loop(){
    char  sendData[100];
    Serial.print("Connect to :");
    Serial.println(serverIP);
    if(l218.connect(serverIP, TCP, 1883)){                    //Connect to server
        Serial.println("Connected !");
    }else{
        Serial.println("Failed to connect");
        while(1);
    }
    delay(200);

    Serial.print("Connect to : ");
    Serial.println(IOT_USERNAME);
    if(l218.MQTTconnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){    //MQTT connect request
        Serial.println("Connected !");
    }else{
        Serial.println("Failed to connect");
        return;
    }
    delay(200);

    Serial.println("Input data : ");
    readSerial(sendData);
    Serial.print("Send data :");
    Serial.print(sendData);
    Serial.println(" ......");
    if(l218.MQTTsend(IOT_TOPIC,sendData)){                    //Send data to topic
        Serial.println("Send OK");
    }else{
        Serial.println("Failed to send");
        return;
    }
    delay(200);

    Serial.print("Subscribe topic : ");
    Serial.println(IOT_TOPIC);
    if(l218.MQTTsubscribe(IOT_TOPIC)){                        //Subscribe topic
        Serial.println("Subscribe OK ! Recive data for three times then unsubscribe");
        int i = 0;
        while(i<3){
            char recvBuff[30] = {0};
            if(l218.MQTTrecv(IOT_TOPIC,recvBuff,30)){         //Recive data from topic
                Serial.print("Recive data :");
                Serial.println(recvBuff);
                i++;
            }
        }
        Serial.print("Unsubscribe topic : ");
        Serial.println(IOT_TOPIC);
        l218.MQTTunsubscribe(IOT_TOPIC);                      //Unsubscribe topic
    }else{
        Serial.println("Fail to subscribe");
        return;
    }
    delay(200);

    Serial.println("Close connection......");
    if(l218.MQTTdisconnect()){                                //MQTT disconnect request
        Serial.println("Close connection !");
    }else{
        Serial.println("Fail to close connection !");
        return;
    }
    delay(2000);
}

int readSerial(char result[]){
    int i = 0;
    while(1){
        while(Serial.available() > 0){
            char inChar = Serial.read();
            if(inChar == '\n'){
                result[i] = '\0';
                Serial.flush();
                return 0;
            }
            if(inChar != '\r'){
                result[i] = inChar;
                i++;
            }
        }
    }
}