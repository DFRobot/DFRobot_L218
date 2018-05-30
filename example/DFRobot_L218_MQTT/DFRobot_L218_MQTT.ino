 /*
  * File  : DFRobot_L218_MQTT.ino
  * Power : L218 powered by 3.7V lithium battery
  * Brief : This example verify MQTT send and subscribe verification
  *         Press the button when the net light blinks L218 start
  *         With initialization completed, we connect to iot.dfrobot.com.cn
  *         Then send data to a topic
  *         Thus we finished the MQTT send verification 
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

#define  BUTTON    3
#define  CHARGE    6
#define  DONE      7
#define  POWER     9

#define serverIP        "iot.dfrobot.com.cn"
#define IOT_CLIENT      " CLIENT NAME "
#define IOT_USERNAME    " USER   NAME "
#define IOT_KEY         " PASSWORD    "
#define IOT_TOPIC       " TOPIC       "

void turn_on()
{  
    if( digitalRead(BUTTON) == LOW ){
        tone(4,2000);
        digitalWrite(POWER,HIGH);
    }else{
        noTone(4);
        digitalWrite(POWER,LOW );
    }
}

void charge()
{
    if(digitalRead(DONE)){
        if( digitalRead(CHARGE) == LOW ){
            tone(4,4000,500);
        }
    }
}


void setup(){
    SerialUSB.begin(115200);
    l218.init();                                                //Initialization

  //L218 boot interrupt. Press the button for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the button until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(BUTTON) , turn_on , CHANGE);

  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE) , charge  , CHANGE);
}

void loop()
{
    if(l218.checkTurnON()){                                     //Check if L218 start
        SerialUSB.println("");
        SerialUSB.println("Turn ON !");
        if(l218.checkSIMcard()){                                //Check SIM card
            SerialUSB.println("Card Init!");
        }else{
            SerialUSB.println("NO SIM card");
            return;
        }
        if(l218.initNetwork()){                                 //Init network functions
           SerialUSB.println("NET ONLINE!");
           delay(5000);
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
        if(l218.MQTTsend(IOT_TOPIC,"L218test")){                //MQTT send data
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