#include "DFRobot_L218getpos.h"

bool   DFRobot_L218getpos::init(void)
{
    int  count = 0;
    while(count < 3){
        if(SIMcore.check_send_cmd("AT+CGATT?\r\n","1")){
            break;
        }else{
            count++;
            delay(500);
        }
    }
    if(count == 3){
        SIMcore.closeCommand();
        return false;
    }
    if(SIMcore.check_send_cmd("AT+CSTT=\"CMNET\"\r\n","OK")){
        if(SIMcore.check_send_cmd("AT+GTPOS=1\r\n","CONNECT OK")){
            SIMcore.setCommandCounter(2);
            return true;
        }else{
            SIMcore.closeCommand();
            return false;
        }
    }else{
        SIMcore.closeCommand();
        return false;
    }
}

bool   DFRobot_L218getpos::getPosition(void)
{
    char  posBuffer[50];
    char *pLongitude,*pLatitude;
    SIMcore.cleanBuffer(posBuffer,50);
    if((SIMcore.getCommandCounter() == 2) || (SIMcore.getCommandCounter() == 5)){
        SIMcore.send_cmd("AT+GTPOS=2\r\n");
        SIMcore.readBuffer(posBuffer,50);
        if(NULL != strstr(posBuffer,"+GTPOS")){
            if(NULL != strstr(posBuffer,"$")){
                SIMcore.setCommandCounter(3);
            }else if(NULL != strstr(posBuffer,"-1")){
                SIMcore.setCommandCounter(6);
            }else if(NULL != strstr(posBuffer,"-2")){
                SIMcore.setCommandCounter(7);
            }else if(NULL != strstr(posBuffer,"-3")){
                SIMcore.setCommandCounter(8);
            }else if(NULL != strstr(posBuffer,"-4")){
                SIMcore.setCommandCounter(9);
            }else if(NULL != strstr(posBuffer,"-5")){
                SIMcore.setCommandCounter(10);
            }else if(NULL != strstr(posBuffer,"-6")){
                SIMcore.setCommandCounter(11);
            }else if(NULL != strstr(posBuffer,"-7")){
                SIMcore.setCommandCounter(12);
            }
        }else{
            SIMcore.closeCommand();
            return false;
        }
    }else{
        SIMcore.closeCommand();
        return false;
    }
    if(SIMcore.getCommandCounter() == 3){
        pLongitude = strstr(posBuffer,":");
        pLatitude  = strstr(posBuffer,",");
        memcpy(longitude,pLongitude+2,7);
        memcpy(latitude, pLatitude+1 ,7);
        SIMcore.setCommandCounter(4);
        return true;
    }
}

char*   DFRobot_L218getpos::getLongitude(void)
{
    if((SIMcore.getCommandCounter() == 4) || (SIMcore.getCommandCounter() == 5)){
        SIMcore.setCommandCounter(5);
        return longitude;
    }else if(SIMcore.getCommandCounter() == 6 ){
        SIMcore.setCommandCounter(2);
        return "Error:Network busy";
    }else if(SIMcore.getCommandCounter() == 7 ){
        SIMcore.setCommandCounter(2);
        return "Error:Not ready";
    }else if(SIMcore.getCommandCounter() == 8 ){
        SIMcore.setCommandCounter(2);
        return "Error:Network error";
    }else if(SIMcore.getCommandCounter() == 9 ){
        SIMcore.setCommandCounter(2);
        return "Error:Timeout";
    }else if(SIMcore.getCommandCounter() == 10){
        SIMcore.setCommandCounter(2);
        return "Error:Unack";
    }else if(SIMcore.getCommandCounter() == 11){
        SIMcore.setCommandCounter(2);
        return "Error:Network exists";
    }else if(SIMcore.getCommandCounter() == 12){
        SIMcore.setCommandCounter(2);
        return "Error:WIFI information error";
    }else{
        return "error";
    }
}

char*   DFRobot_L218getpos::getLatitude(void)
{
    if((SIMcore.getCommandCounter() == 4) || (SIMcore.getCommandCounter() == 5)){
        SIMcore.setCommandCounter(5);
        return latitude;
    }else if(SIMcore.getCommandCounter() == 6 ){
        SIMcore.setCommandCounter(2);
        return "Error:Network busy";
    }else if(SIMcore.getCommandCounter() == 7 ){
        SIMcore.setCommandCounter(2);
        return "Error:Not ready";
    }else if(SIMcore.getCommandCounter() == 8 ){
        SIMcore.setCommandCounter(2);
        return "Error:Network error";
    }else if(SIMcore.getCommandCounter() == 9 ){
        SIMcore.setCommandCounter(2);
        return "Error:Timeout";
    }else if(SIMcore.getCommandCounter() == 10){
        SIMcore.setCommandCounter(2);
        return "Error:Unack";
    }else if(SIMcore.getCommandCounter() == 11){
        SIMcore.setCommandCounter(2);
        return "Error:Network exists";
    }else if(SIMcore.getCommandCounter() == 12){
        SIMcore.setCommandCounter(2);
        return "Error:WIFI information error";
    }else{
        return "error";
    }
}
