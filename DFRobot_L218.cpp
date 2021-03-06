#include <DFRobot_L218.h>

void   DFRobot_L218::init(void)
{
    Serial1.begin(9600);
    pinMode(3       , INPUT );
    pinMode(6       , INPUT );
    pinMode(7       , INPUT );
    pinMode(8       , INPUT );
    pinMode(A0      , INPUT );
    pinMode(A2      , INPUT );
    pinMode(9       , OUTPUT);
    pinMode(12      , OUTPUT);
    pinMode(13      , OUTPUT);
    pinMode(A1      , OUTPUT);
    pinMode(A3      , OUTPUT);
    pinMode(A4      , OUTPUT);
    pinMode(A5      , OUTPUT);
    digitalWrite(A1 , LOW   );
    digitalWrite(A3 , HIGH  );
    digitalWrite(A4 , HIGH  );
    digitalWrite(A5 , HIGH  );
    digitalWrite(12 , LOW   );
    digitalWrite(13 , LOW   );
}

bool   DFRobot_L218::turnON(void)
{
    digitalWrite(A4 , HIGH  );
    digitalWrite( 9 , HIGH  );
    delay(2000);
    digitalWrite( 9 , LOW   );
    int i = 3;
    while(i){
        if(digitalRead(A2)){
            return true;
        }
        delay(500);
        i--;
    }
    return false;
}

bool   DFRobot_L218::checkTurnON(void)
{
    return  digitalRead(A2);
}

int   DFRobot_L218::checkBattery(void)
{
    digitalWrite(A1 , LOW   );
    int   sensorValue = analogRead(A0);
    float voltage     = sensorValue * (3.3 / 1023.0);
    sensorValue = 0;
    if(voltage > 2.03){
        sensorValue = 100;
    }
    if(2.03 > voltage > 1.99){
        sensorValue =  90;
    }
    if(2.00 > voltage > 1.96){
        sensorValue =  80;
    }
    if(1.97 > voltage > 1.93){
        sensorValue =  70;
    }
    if(1.94 > voltage > 1.91){
        sensorValue =  60;
    }
    if(1.92 > voltage > 1.89){
        sensorValue =  50;
    }
    if(1.90 > voltage > 1.88){
        sensorValue =  40;
    }
    if(1.89 > voltage > 1.87){
        sensorValue =  30;
    }
    if(1.88 > voltage > 1.84){
        sensorValue =  20;
    }
    if(1.85 > voltage){
        sensorValue =  10;
    }
    return sensorValue;
}

bool   DFRobot_L218::checkSIMcard(void)
{
    int count = 0;
    delay(1000);
    while(count < 3){
        if(digitalRead(A2)){
            break;
        }else{
            count++;
            delay(300);
        }
    }
    if(count == 3){
        return false;
    }
    count = 0;
    while(count < 3){
        if(check_send_cmd("AT+CPIN?\r\n","READY")){
            break;
        }else{
            count++;
            delay(300);
        }
    }
    if(count == 3){
        return false;
    }
    return true;
}

void   DFRobot_L218::turnOFF(void)
{
    Serial1.println("at+cpowd=1");
    digitalWrite(A3,  LOW  );
    digitalWrite(A4 , LOW  );
    digitalWrite(A5,  LOW  );
}

void   DFRobot_L218::blink(int times , int interval)
{
    while(times){
        digitalWrite(13 , HIGH);
        delay(interval);
        digitalWrite(13 , LOW );
        delay(interval);
        times--;
    }
}

bool   DFRobot_L218::voiceCall(const char* phoneNumber)
{
    Serial1.write("ATD");
    Serial1.write(phoneNumber);
    if(check_send_cmd(";\r\n","OK")){
        return true;
    }else{
        return false;
    }
}

bool   DFRobot_L218::beginSMS(const char* phoneNumber)
{
    if(check_send_cmd("AT+CMGF=1\r\n","OK")){
        delay(100);
    }else{
        return false;
    }
    Serial1.write("AT+CMGS=\"");
    Serial1.write(phoneNumber);
    if(check_send_cmd("\"\r\n",">")){
        delay(100);
        return true;
    }else{
        return false;
    }
}

bool   DFRobot_L218::sendSMS(const char* content)
{
    Serial1.write(content);
    if(check_send_cmd("","+CMGS",900,5000)){
        return true;
    }else{
        return false;
    }
}

bool   DFRobot_L218::initPos(void)
{
    int  count = 0;
    if(check_send_cmd("AT+CSTT=\"CMNET\"\r\n","OK")){
        if(check_send_cmd("AT+ciicr\r\n","OK",1000,3000)){
            if(check_send_cmd("AT+GTPOS=1\r\n" ,"OK",1000,3000)){
                check_send_cmd("AT+GTPOS=2\r\n","OK",1000,3000);
                check_send_cmd("AT+CIPSHUT\r\n","OK",1000,3000);
                if(check_send_cmd("AT+EGDCONT=2,\"IP\" ,\"CMNET\"\r\n","OK")){
                    check_send_cmd("AT+EGDCONT=1,\"IP\",\"CMNET\"\r\n","OK");
                    delay(500);
                    check_send_cmd("AT+MGPSTS=1\r\n","1");
                    delay(500);
                    check_send_cmd("AT+MGPSEPO=1\r\n","1");
                    delay(500);
                    check_send_cmd("AT+GETGPS=1\r\n","1");
                    delay(500);
                    check_send_cmd("AT+MGPSC=1\r\n","1");
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool   DFRobot_L218::getPos(void)
{
    delay(2000);
    char     posBuffer[200];
    char     latitude[10]={0};
    char     longitude[10]={0};
    char    *pLongitude,*pLatitude;
    int      latitude_s =0,longitude_s=0;
    cleanBuffer(posBuffer,200);
    Serial1.write("AT+GETGPS=\"GNRMC\"\r\n");
    readBuffer(posBuffer,200);
  //SerialUSB.println(posBuffer);
    if(NULL != strstr(posBuffer,",A,")){
        if(NULL != strstr(posBuffer,",N,")){
            pLatitude   = strstr(posBuffer,",N,") -  9;
            latitude_s  = 0;
        }else{
            pLatitude   = strstr(posBuffer,",S,") -  9;
            latitude_s  = 1;
        }
        if(NULL != strstr(posBuffer,",E,")){
            pLongitude  = strstr(posBuffer,",E,") - 10;
            longitude_s = 0;
        }else{
            pLongitude  = strstr(posBuffer,",W,") - 10;
            longitude_s = 1;
        }
        memcpy(longitude,pLongitude,10);
        memcpy(latitude ,pLatitude ,10);
        latitude_m   = (latitude[2] -48)*100000 + (latitude[3] -48)*10000+(latitude[5] -48)*1000 + (latitude[6] -48)*100 +(latitude[7] -48)*10+(latitude[8] -48);
        latitude_m  /= 600000;
        latitude_m  +=(latitude[0] -48)*10 +(latitude[1] -48);
        longitude_m  = (longitude[3] -48)*100000 + (longitude[4] -48)*10000+(longitude[6] -48)*1000 + (longitude[7] -48)*100 +(longitude[8] -48)*10+(longitude[9] -48);
        longitude_m /= 600000;
        longitude_m +=(longitude[0] -48)*100 +(longitude[1] -48)*10+(longitude[2] -48);
        if(latitude_s){
            latitude_m  = 0 - latitude_m ;
        }
        if(longitude_s){
            longitude_m = 0 - longitude_m;
        }
        return true;
    }else{
        return false;
    }
}

double DFRobot_L218::getLongitude()
{
    return  longitude_m;
}

double DFRobot_L218::getLatitude()
{
    return  latitude_m;
}

bool   DFRobot_L218::connect(char *server,Protocol ptl,int port)
{
    char num[4];
    char serverIP[100];
    cleanBuffer(serverIP,100);
    itoa(port, num, 10);
    Serial1.write("AT+CIPSTART=\"");
    if(ptl == TCP){
        Serial1.write("TCP\",\"");
    }else if(ptl == UDP){
        Serial1.write("UDP\",\"");
    }else{
        SerialUSB.println("Wrong protocol");
        return false;
    }
    Serial1.write(server);
    Serial1.write("\",");
    Serial1.write(num);
    Serial1.write("\r\n");
    while(1){
        while(Serial1.available()){
            readBuffer(serverIP,100);
            if(NULL != strstr(serverIP,"ALREADY CONNECT")){
                return true;
            }
            if(NULL != strstr(serverIP,"CONNECT OK")){
                return true;
            }
            if(NULL != strstr(serverIP,"CONNECT FAIL")){
                return false;
            }
        }
    }
}

bool   DFRobot_L218::disconnect(void)
{
    if(check_send_cmd("AT+CIPSHUT\r\n","OK",1000,3000)){
        Serial1.println("AT+CIPCLOSE");
        return true;
    }else{
        return false;
    }
}

void   DFRobot_L218::sleepMode(void)
{
    digitalWrite(A5,LOW);
    Serial1.println("AT+CFUN=0");
    delay(100);
    Serial1.println("AT+ESLP=1");
    delay(100);
    Serial1.println("AT+CSCLK=1");
    delay(100);
    Serial1.end();
    Serial1.begin(19200);
}

void   DFRobot_L218::wakeUp(void)
{
    digitalWrite(A5,HIGH);
    Serial1.println("AT+CFUN=1");
}

int    DFRobot_L218::readBuffer(char *buffer, int count, unsigned int timeout, unsigned int chartimeout)
{
    int i = 0;
    unsigned long timerStart, prevChar;
    timerStart = millis();
    prevChar = 0;
    while(1){
        while(Serial1.available()){
            buffer[i++] = Serial1.read();
            prevChar = millis();
            if(i >= count){
                return i;
            }
        }
        if(timeout){
            if((unsigned long) (millis() - timerStart) > timeout*10){
                break;
            }
        }
        if(((unsigned long) (millis() - prevChar) > chartimeout) && (prevChar != 0)){
            break;
        }
    }
    Serial1.flush(); 
    return i;
}

void   DFRobot_L218::cleanBuffer(char *buffer,int count)
{
    for(int i=0; i < count; i++){
        buffer[i] = '\0';
    }
}

bool   DFRobot_L218::initNetwork(void)
{
    int  count = 0;
    while(count < 3){
        if(check_send_cmd("AT+CGATT?\r\n","1")){
            break;
        }else{
            count++;
            delay(500);
        }
    }
    if(count == 3){
        return false;
    }
    if(check_send_cmd("AT+CSTT\r\n","OK")){
        if(check_send_cmd("AT+CIICR\r\n","OK")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool   DFRobot_L218::mqttConnect(char* iot_client, char* iot_username, char* iot_key)
{
    if(check_send_cmd("AT+CIPSEND\r\n",">")){
        char M0buffer[50]   = {0};
        char MQTThead[10]={0x00,0x04,0x4d,0x51,0x54,0x54,0x04,0xc2,0x0b,0xb8};
        Serial1.write(0x10);
        int leng = 10;
        leng += strlen(iot_key)     +2;
        leng += strlen(iot_client)  +2;
        leng += strlen(iot_username)+2;
        Serial1.write(leng);
        for(int i=0;i<10;i++){
            Serial1.write(MQTThead[i]);
        }
        Serial1.write(MQTThead[0]);
        Serial1.write(strlen(iot_client));
        Serial1.print(iot_client);
        Serial1.write(MQTThead[0]);
        Serial1.write(strlen(iot_username));
        Serial1.print(iot_username);
        Serial1.write(MQTThead[0]);
        Serial1.write(strlen(iot_key));
        Serial1.print(iot_key);
        Serial1.write(0x1a);
        readBuffer(M0buffer,50,5000);
        return true;
    }
    return false;
}

bool   DFRobot_L218::mqttPublish(char* iot_topic, String iot_data)
{
        char M0buffer[50]   = {0};
        Serial1.print("AT+CIPSEND\r\n");
        readBuffer(M0buffer,50,5000);
    if(NULL != strstr(M0buffer,">")){
        char     MQTTdata[2]={0x00,0x04};
        int leng = 0;
        Serial1.write(0x32);
        leng = strlen(iot_topic)+iot_data.length()+4;
        Serial1.write(leng);
        Serial1.write(MQTTdata[0]);
        Serial1.write(strlen(iot_topic));
        Serial1.print(iot_topic);
        for(int j=0;j<2;j++){
            Serial1.write(MQTTdata[j]);
        }
        Serial1.print(iot_data);
        Serial1.write(0x1a);
        readBuffer(M0buffer,50,5000);
        return true;
    }else{
        return false;
    }
}

bool   DFRobot_L218::mqttSubscribe(char* iot_topic)
{
    if(check_send_cmd("AT+CIPSEND\r\n",">",900,5000)){
        char     MQTTbuff[10]={0};
        MQTTbuff[0] = 0x82;
        MQTTbuff[1] = strlen(iot_topic)+5;
        MQTTbuff[3] = 0x0a;
        MQTTbuff[5] = strlen(iot_topic);
        Serial1.write(MQTTbuff,6);
        Serial1.write(iot_topic);
        MQTTbuff[0] = 0x01;
        Serial1.write(MQTTbuff,1);
        if(check_send_cmd("","CLOSED")){
            return false;
        }else{
            return true;
        }
    }else{
        return false;
    }
}

bool   DFRobot_L218::mqttUnsubscribe(char* iot_topic)
{
    if(check_send_cmd("AT+CIPSEND\r\n",">")){
        char     MQTTbuff[10]={0};
        MQTTbuff[0] = 0xa2;
        MQTTbuff[1] = strlen(iot_topic)+4;
        MQTTbuff[3] = 0x0a;
        MQTTbuff[5] = strlen(iot_topic);
        Serial1.write(MQTTbuff,6);
        Serial1.write(iot_topic);
        if(check_send_cmd("","CLOSED")){
            return false;
        }else{
            return true;
        }
    }else{
        return false;
    }
}

bool   DFRobot_L218::mqttRecv(char* iot_topic, char* buf, int maxlen)
{
    char   MQTTbuff[maxlen+30];
    char  *p; 
    cleanBuffer(MQTTbuff,maxlen+30);
    int i = readBuffer(MQTTbuff,maxlen+30);
    for(int j=0;j<i;j++){
        if(NULL != (p = strstr(MQTTbuff+j,iot_topic))){
            memcpy(buf,p+strlen(iot_topic),maxlen+30);
            return true;
        }
    }
    return false;
}

bool   DFRobot_L218::mqttDisconnect(void)
{
    if(check_send_cmd("AT+CIPSEND\r\n",">")){
        char     MQTTdata[2]={0xe0,0x00};
        Serial1.write(MQTTdata,2);
        Serial1.write(0x1a);
        return true;
    }else{
        return false;
    }
}

bool   DFRobot_L218::httpInit(void)
{
    if(check_send_cmd("AT+SAPBR=3,1,\"APN\",\"cmnet\"\r\n","OK",10,1000)){
        if(!check_send_cmd("AT+SAPBR=1,1\r\n","OK",10,1000)){
            return false;
        }
    }else{
         return false;
    }
    if(!check_send_cmd("AT+SAPBR=2,1\r\n","OK",10,1000)){
        return false;
    }
    return true;
}

bool  DFRobot_L218::httpConnect(const char *Host)
{
    char httpbuff[100];
    int k = 0;
    delay(5);
    Serial1.write("AT+HTTPPARA=URL,\"");
    Serial1.write(Host);
    Serial1.write("\"\r\n");
    while(1){
        while(Serial1.available()){
            readBuffer(httpbuff,100);
            if(NULL != strstr(httpbuff,"OK")){
                k = 1;
                break;
            }
            if(NULL != strstr(httpbuff,"ERROR")){
                return false;
            }
        }
        if(k){
            break;
        }
    }
    Serial1.write("AT+HTTPSETUP\r\n");
    while(1){
        if(Serial1.available()){
            readBuffer(httpbuff,100);
            if(NULL != strstr(httpbuff,"OK")){
                return true;
            }
            if(NULL != strstr(httpbuff,"ERROR")){
                SerialUSB.println("Fail to set up http connect");
                return false;
            }
        }
    }
}

bool  DFRobot_L218::httpPost(const char *Host , String data)
{
    if(!httpConnect(Host)){
        return false;
    }
    Serial1.write("AT+HTTPACTION=2,");
    String    length;
    length += data.length();
    Serial1.print(length);
    Serial1.write(",\"");
    Serial1.print(data);
    Serial1.write("\"\r\n");
    get_String();
    SerialUSB.println();
    return true;
}

void   DFRobot_L218::httpGet(const char *Host)
{
    if(!httpConnect(Host)){
        return;
    }
    Serial1.write("AT+HTTPACTION=0\r\n");
    get_String();
    SerialUSB.println();
}

void   DFRobot_L218::httpDisconnect(void)
{
    Serial1.write("AT+HTTPCLOSE\r\n");
}

bool   DFRobot_L218::check_send_cmd(const char* cmd, const char* resp, unsigned int timeout, unsigned int chartimeout)
{
    char SIMbuffer[100];
    cleanBuffer(SIMbuffer,100);
    Serial1.write(cmd);
    readBuffer(SIMbuffer,100,timeout, chartimeout);
    if(NULL != strstr(SIMbuffer,resp)){
        return true;
    }else{
        return false;
    }
}

void DFRobot_L218::get_String(void)
{
    uint64_t t1 = millis();
    while(1){
        while(Serial1.available()){
            SerialUSB.print(char(Serial1.read()));
            t1 = millis();
        }
        if((millis() - t1) > 10000){
            return;
        }
    }
}