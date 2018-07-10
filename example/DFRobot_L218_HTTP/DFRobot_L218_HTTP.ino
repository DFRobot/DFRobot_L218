 /*
  * File  : DFRobot_L218_HTTP.ino
  * Power : L218 powered by 3.7V lithium battery
  * Brief : This example verify HTTP connection over a HTTP request
  *         Press the button when the net light blinks L218 start
  *         With initialization completed, we connect to server POST data and GET data
  *         Thus we finished the HTTP POST and GET verification
  */

#include <DFRobot_L218.h>

DFRobot_L218  l218;

#define  BUTTON_PIN    3
#define  CHARGE_PIN    6
#define  DONE_PIN      7
#define  POWER_PIN     9

//Login website (https://www.tlink.io/) to register an account ,fill the following information based on your account
#define deviceNo  "DEVICE ID"
#define sensorsId "SENSOR ID"
#define value     "  VALUE  "

//This URL is use for post data to tlink
#define POSTURL   "api.tlink.io/tlink_interface/api/device/createDataPonit.htm"
//This URL is use for get data from tlink, please change the SENSORID to your sensorsId
#define GETURL    "api.tlink.io/tlink_interface/api/device//getDataPoint_SENEORID.htm"

void turn_on()
{
    static int   t1=0,t2=0;
    t1=t2;
    t2=millis();
    if(t1-t2){
        if( digitalRead(BUTTON_PIN) == LOW ){
            tone(4,2000);
            digitalWrite(POWER_PIN,HIGH);
        }else{
            noTone(4);
            digitalWrite(POWER_PIN,LOW );
        }
    }else{
        noTone(4);
    }
}

void charge()
{
    if(digitalRead(DONE_PIN) ){
        if( digitalRead(CHARGE_PIN) == LOW ){
            tone(4,4000,500);
        }
    }
}

void setup(){
    SerialUSB.begin(115200);
    l218.init();                                                //Initialization
  //L218 boot interrupt. Press the button for 1-2 seconds, L218 turns on when NET LED light up, Press and hold the button until the NET LED light off L218 turns off.
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN) , turn_on , CHANGE);
  //Battery charge interrupt. When battery get charge from USB, Buzzer sounds for 0.5 seconds
    attachInterrupt(digitalPinToInterrupt(CHARGE_PIN) , charge  , CHANGE);
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
        if(l218.httpInit()){                                    //Init http service
            SerialUSB.println("HTTP init !");
        }else{
            SerialUSB.println("Fail to init http");
            return;
        }
        SerialUSB.print("Post to ");
        SerialUSB.println(POSTURL);
        String httpbuff;
        httpbuff += "{\'deviceNo\':\'";                         //{
        httpbuff += deviceNo;                                   //   "deviceNo" : "DEVICE NO",
        httpbuff += "\',\'sensorDatas\':[{\'sensorsId\':";      //      "sensorDatas":[{
        httpbuff += sensorsId;                                  //          "sensorsId" :  SENSOR ID,
        httpbuff += ",\'value\':\'";                            //          "value"     : "  VALUE  "
        httpbuff += value;                                      //       }]
        httpbuff += "\'}]}";                                    //}
        while(1){
            if(l218.httpPost(POSTURL,httpbuff)){                //HTTP POST
                Serial.println("Post successed");
                break;
            }else{
                Serial.println("Fail to post");
            }
        }
        SerialUSB.print("Get from ");
        SerialUSB.println(GETURL);
        l218.httpGet(GETURL);                                   //HTTP GET
        SerialUSB.println("Disconnect");
        l218.httpDisconnect();                                  //HTTP Disconnect
    }else{
        SerialUSB.println("Please Turn ON L218");
        delay(3000);
    }
}