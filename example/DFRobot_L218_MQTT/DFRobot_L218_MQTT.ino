 /*
  * File  : DFRobot_L218_MQTT.ino
  * Power : L218 needs 3.3V DC power supply
  * Brief : This example verify MQTT send and subscribe verification
  *         Press the button when the net light blinks L218 start
  *         With initialization completed, we connect to iot.dfrobot.com.cn
  *         Then send data to a topic
  *         Thus we finished the MQTT send subscribe verification 
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

#define serverIP        "iot.dfrobot.com.cn"
#define IOT_CLIENT      "test"
#define IOT_USERNAME    "B1WMsSlvof"
#define IOT_KEY         "BkMGjHxvjz"
#define IOT_TOPIC       "BJ7eUeDoM"

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

void ring()
{
    if( digitalRead(Ring) == LOW ){
        tone(4,8000);
        SerialUSB.println("Ring ! ! !");
    }else{
        noTone(4);
        SerialUSB.println("Ring Over");
    }
}

void setup(){
    SerialUSB.begin(115200);
    l218.init();
    attachInterrupt(digitalPinToInterrupt(Button) , turn_on , CHANGE);  //L218 boot interrupt
    attachInterrupt(digitalPinToInterrupt(Charge) , charge  , CHANGE);  //Battery charge interrupt
}

void loop()
{
    delay(5000);
    if(l218.check_TurnON()){                                            //Check if L218 start
        SerialUSB.println("");
        SerialUSB.println("Turn ON !");
        if(l218.check_SIMcard()){                                       //Check SIM card
            SerialUSB.println("Card Init!");
        }else{
            SerialUSB.println("NO SIM card");
            return;
        }
        if(l218.initNet()){                                             //Init network functions
           SerialUSB.println("NET ONLINE!");
           delay(5000);
        }else{
            SerialUSB.println("NO NET");
            return;
        }
        if(l218.connect(serverIP,TCP,1883)){                            //Connect to server
            SerialUSB.println("connected");
        }else{
            SerialUSB.println("disconnect");
            return;
        }
        if(l218.MQTTconnect(IOT_CLIENT,IOT_USERNAME,IOT_KEY)){          //MQTT connect requst
            SerialUSB.println("MQTT connected");
            delay(2000);
        }else{
            SerialUSB.println("MQTT connect failed");
            return;
        }
        if(l218.MQTTsend(IOT_TOPIC,"L218test")){                        //MQTT send data
            SerialUSB.println("Send OK");
        }else{
            SerialUSB.println("MQTT fail to send");
            return;
        }
        if(l218.MQTTdisconnect()){
            SerialUSB.println("MQTT disconnected");
        }else{
            SerialUSB.println("Fail to disconnect");
        }
        if(l218.disconnect()){
            SerialUSB.println("Server disconnected");
        }else{
            SerialUSB.println("Fail to disconnect");
        }
    }
}