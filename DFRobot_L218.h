#ifndef __DFRobot_L218_H__
#define __DFRobot_L218_H__

#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>
#include <stdio.h>

#define  Button    3
#define  Charge    6
#define  Done      7
#define  Ring      8
#define  power     9
#define  MPU      12
#define  Reset    A3
#define  Led      13
#define  WK_UP    A5


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
    void    startMPU6050(void);
    void    stopMPU6050(void);
    void    sleepMode(void);
    void    wakeUp(void);
    bool    turnON(void);
    bool    check_TurnON(void);
    bool    getPos(void);
    bool    initPos(void);
    bool    check_SIMcard(void);
    bool    initNet(void);
    bool    beginSMS(const char* phoneNumber);
    bool    sendSMS(const char* content);
    bool    connect(char *server,Protocol ptl,int port);
    bool    disconnect(void);
    bool    MQTTconnect(char* iot_client, char* iot_username, char* iot_key);
    bool    MQTTsend(char* iot_topic, char* iot_data);
    bool    MQTTsubscribe(char* iot_topic);
    bool    MQTTunsubscribe(char* iot_topic);
    bool    MQTTrecv(char* iot_topic, char* buf, int maxlen);
    bool    MQTTdisconnect(void);
    bool    voiceCall(const char* phoneNumber);
    bool    check_send_cmd(const char* cmd, const char* resp, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
    int     readBuffer(char *buffer, int count, unsigned int timeout = DEFAULT_TIMEOUT, unsigned int chartimeout = DEFAULT_INTERCHAR_TIMEOUT);
    double  getLongitude(void);
    double  getLatitude(void);
private:
    double  longitude_m = 0;
    double  latitude_m  = 0;

};

#endif