 /*
  * File   : DFRobot_Demo.ino
  * Power : L218 powered by 3.7V lithium battery
  * Brief  : This example use L218 get position tempeeraturw then store the data in SD card
  *          And send the data to Net whlie MQTT
  * Note   : The tracker function only available in outdoor
  */

#include <MPU6050.h>
#include <DFRobot_L218.h>
#include <avr/dtostrf.h>

MPU6050          mpu;
DFRobot_L218    l218;

#define serverIP        "iot.dfrobot.com.cn"
#define IOT_CLIENT      " CLIENT NAME "
#define IOT_USERNAME    " USER   NAME "
#define IOT_KEY         " PASSWORD    "
#define IOT_TOPIC       " TOPIC       "

void turn_on()  
{  
    if( digitalRead(Button) == LOW ){
        tone(4,2000);
        digitalWrite(power,HIGH);
    }else{
        noTone(4);
        digitalWrite(power,LOW);
    }
}

void charge()
{
    if(digitalRead(Done)){
        if( digitalRead(Charge) == LOW ){
            tone(4,4000,500);
        }
    }
}

void setup(){
    SerialUSB.begin(115200);
    l218.init();                                                //Initialization

  //L218 boot interrupt. Press the button for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the button until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(Button) , turn_on , CHANGE);

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(Charge) , charge  , CHANGE);
}

void loop(){
    if(l218.checkTurnON()){
        double  Longitude,Latitude;
        SerialUSB.println("Turn ON !");
        delay(500);
        if(l218.initPos()){                                     //Init positioning functions
            SerialUSB.println("Init position");
            delay(500);
        }else{
            SerialUSB.println("Not init position");
            return;
        }
         while(1){
            delay(500);
            if(l218.getPos()){                                  //Get location information
                SerialUSB.println("Get position");
                Longitude = l218.getLongitude();                //Get longitude
                Latitude  = l218.getLatitude();                 //Get latitude
                SerialUSB.print("Longitude = ");
                SerialUSB.print(Longitude,6);
                SerialUSB.print("Latitude  = ");
                SerialUSB.print(Latitude ,6);
                break;
            }else{
                SerialUSB.println("Not position");
            }               
        }
       l218.startMPU6050();
        SerialUSB.println("Initialize MPU6050");
        while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
            SerialUSB.println("Could not find a valid MPU6050 sensor, check wiring!");
            delay(500);
        }
        float temp = mpu.readTemperature();
        SerialUSB.print(" Temp = ");
        SerialUSB.print(temp);
        SerialUSB.println(" *C");
        delay(500);
        l218.stopMPU6050();
        char  L218buffer[80] = {0};
        char   latitude[30];
        char   longitude[30];
        char temperature[15] = {0};
        dtostrf(temp      , 4 , 2 , temperature);
        dtostrf(Longitude , 8 , 5 , longitude  );
        dtostrf(Latitude  , 7 , 5 , latitude   );
        memcpy(L218buffer,"Longitude : ",12);
        strcat(L218buffer, longitude);
        strcat(L218buffer," Latitude : ");
        strcat(L218buffer,latitude);
        strcat(L218buffer," Temperature: ");
        strcat(L218buffer,temperature);
        delay(500);
        SerialUSB.print("L218buffer = ");
        SerialUSB.println(L218buffer);
        if(l218.checkSIMcard()){                                //Check SIM card
            SerialUSB.println("Card ready");
            delay(500);
        }else{
            SerialUSB.println("NO Card");
            return;
        }
        if(l218.initNetwork()){                                 //Init network functions
           SerialUSB.println("NET ONLINE!");
           delay(2000);
        }else{
            SerialUSB.println("NO NET");
            return;
        }
        if(l218.connect(serverIP,TCP,1883)){                    //Connect to server
            SerialUSB.println("connected");
        }else{
            SerialUSB.println("disconnect");
            return;
        }
        if(l218.MQTTconnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){  //MQTT connect requst
            SerialUSB.println("MQTT connected");
            delay(2000);
        }else{
            SerialUSB.println("MQTT connect failed");
            return;
        }
        if(l218.MQTTsend(IOT_TOPIC,L218buffer)){                //MQTT send data
            SerialUSB.println("Send OK");
        }else{
            SerialUSB.println("MQTT fail to send");
            return;
        }
        if(l218.MQTTdisconnect()){                              //MQTT disconnect requst
            SerialUSB.println("MQTT disconnected");
        }else{
            SerialUSB.println("Fail to disconnect");
        }
        if(l218.disconnect()){                                  //Disconnect from server
            SerialUSB.println("Server disconnected");
        }else{
            SerialUSB.println("Fail to disconnect");
        }
    }else{
        SerialUSB.println("Please Turn ON L218");
        delay(3000);
    }
}