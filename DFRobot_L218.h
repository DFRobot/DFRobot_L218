#ifndef __DFRobot_L218_H__
#define __DFRobot_L218_H__

#include "DFRobot_SIM.h"

enum Mode{
    Normal,
    Hex,
};

class DFRobot_L218 : public DFRobot_SIMcore , public DFRobot_SIMphonecall , public DFRobot_SIMsms , public DFRobot_SIMclient
{
public:
    bool     initFile(void);
    char*    getList(void);
    char*    getList(char *foldername);
    char*    getSpace(void);
    bool     createFile(char *filename);
    char*    readFile(char *filename,int offset = 0, int len = 50, Mode mode = Normal);
    bool     deleteFile(char *filename);
    bool     writeFile(char *filename, char *data, Mode mode = Normal);
    bool     createFolder(char *foldername);
    bool     deleteFolder(char *foldername);
    bool     MQTTconnect(char* iot_client, char* iot_username, char* iot_key);
    bool     MQTTsend(char* iot_topic, char* iot_data);
    bool     MQTTsubscribe(char* iot_topic);
    bool     MQTTunsubscribe(char* iot_topic);
    bool     MQTTrecv(char* iot_topic, char* buf,int maxlen);
    bool     MQTTdisconnect(void);
    bool     initPos(void);
    bool     getPosition(void);
    char*    getLongitude(void);
    char*    getLatitude(void);
private:
    char     latitude[8];
    char     longitude[8];
};

#endif