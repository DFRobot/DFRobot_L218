#ifndef __DFRobot_L218_H__
#define __DFRobot_L218_H__

#include <Arduino.h>
//#include <stdint.h>
//#include <Wire.h>
//#include <stdio.h>

#define  DEFAULT_TIMEOUT               5
#define  DEFAULT_INTERCHAR_TIMEOUT     500

enum Protocol {
    TCP    = 1,
    UDP    = 2,
};


class DFRobot_L218
{

public:
    void    init(void);
    void    cleanBuffer(char *buffer,int count);
    void    blink(int times , int interval);
    void    sleepMode(void);
    void    wakeUp(void);
    void    turnOFF(void);
    bool    turnON(void);
    bool    checkTurnON(void);
    bool    getPos(void);
    bool    initPos(void);
    bool    checkSIMcard(void);
    bool    initNetwork(void);
    bool    beginSMS(const char* phoneNumber);
    bool    sendSMS(const char* content);
    bool    connect(char *server,Protocol ptl,int port);
    bool    disconnect(void);
    bool    mqttConnect(char* iot_client, char* iot_username, char* iot_key);
    bool    mqttPublish(char* iot_topic, String iot_data);
    bool    mqttSubscribe(char* iot_topic);
    bool    mqttUnsubscribe(char* iot_topic);
    bool    mqttRecv(char* iot_topic, char* buf, int maxlen);
    bool    mqttDisconnect(void);
    bool    httpInit(void);
    bool    httpConnect(const char *Host);
    bool    httpPost(const char *Host , String data);
    void    httpGet(const char *Host);
    void    httpDisconnect(void);
    void    get_String(void);
    bool    voiceCall(const char* phoneNumber);
    bool    check_send_cmd(const char* cmd, const char* resp, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
    int     readBuffer(char *buffer, int count, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
    int     checkBattery(void);
    double  getLongitude(void);
    double  getLatitude(void);

private:
    double  longitude_m = 0;
    double  latitude_m  = 0;

};

#endif