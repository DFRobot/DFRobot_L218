#include <DFRobot_L218.h>

bool  DFRobot_L218::initFile(void)
{
    if(!SIMcore.check_send_cmd("AT+FSCREATE=?\r\n","OK")){
        SIMcore.setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!SIMcore.check_send_cmd("AT+FSREAD=?\r\n","OK")){
        SIMcore.setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!SIMcore.check_send_cmd("AT+FSWRITE=?\r\n","OK")){
        SIMcore.setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!SIMcore.check_send_cmd("AT+FSMKDIR=?\r\n","OK")){
        SIMcore.setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!SIMcore.check_send_cmd("AT+FSRMDIR=?\r\n","OK")){
        SIMcore.setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!SIMcore.check_send_cmd("AT+FSLS=?\r\n","OK")){
        SIMcore.setCommandCounter(0);
        return false;
    }
    SIMcore.setCommandCounter(1);
    return true;
}

char* DFRobot_L218::getList(void)
{
    if(SIMcore.getCommandCounter() == 1){
        char SIMList[300];
        SIMcore.cleanBuffer(SIMList,300);
        SIMcore.send_cmd("AT+FSLS\r\n");
        SIMcore.readBuffer(SIMList,300);
        return SIMList+9;
    }else{
        return "error";
    }
}

char* DFRobot_L218::getList(char *foldername)
{
    if(SIMcore.getCommandCounter() == 1){
        char SIMList[300];
        SIMcore.cleanBuffer(SIMList,300);
        SIMcore.send_cmd("AT+FSLS=\"");
        SIMcore.send_cmd(foldername);
        SIMcore.send_cmd("\"\r\n");
        SIMcore.readBuffer(SIMList,300);
        return SIMList+8;
    }else{
        return "error";
    }
}

char* DFRobot_L218::getSpace(void)
{
    if(SIMcore.getCommandCounter() == 1){
        char fileSpace[50];
        SIMcore.cleanBuffer(fileSpace,50);
        SIMcore.send_cmd("AT+FSINFO=\"Z:\"\r\n");
        SIMcore.readBuffer(fileSpace,50);
        return fileSpace+18;
    }else{
        return "error";
    }
}

bool  DFRobot_L218::createFile(char *filename)
{
    if(SIMcore.getCommandCounter() == 1){
        SIMcore.send_cmd("AT+FSCREATE=\"");
        SIMcore.send_cmd(filename);
        if(SIMcore.check_send_cmd("\"\r\n","OK")){
            return true;
        }else{
            return false;
        }    
    }else{
        return false;
    }
}

char* DFRobot_L218::readFile(char *filename, int offset, int len, Mode mode)
{
    if(SIMcore.getCommandCounter() == 1){
        int   rOffset = len+strlen(filename)+6;
        rOffset *=2;
        char  L218file[rOffset+50];
        SIMcore.cleanBuffer(L218file,rOffset+50);
        char  num1[4],num2[4];
        itoa(offset, num1, 10);
        itoa(len,    num2, 10);
        if(mode == Normal){
            SIMcore.send_cmd("AT+FSREAD=\"");
        }else if(mode == Hex){
            SIMcore.send_cmd("AT+FSREADHEX=\"");
        }else{
            return "No such mode";
        }
        SIMcore.send_cmd(filename);
        SIMcore.send_cmd("\",");
        SIMcore.send_cmd(num1);
        SIMcore.send_cmd(",");
        SIMcore.send_cmd(num2);
        SIMcore.send_cmd("\r\n");
        SIMcore.readBuffer(L218file,rOffset+50);
        if(mode == Normal){
            return L218file+10;
        }
        if(mode == Hex){
            return L218file+13;
        }
    }else{
        return "error";
    }
}

bool  DFRobot_L218::deleteFile(char *filename)
{
    if(SIMcore.getCommandCounter() == 1){
        SIMcore.send_cmd("AT+FSDEL=\"");
        SIMcore.send_cmd(filename);
        if(SIMcore.check_send_cmd("\"\r\n","OK")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool  DFRobot_L218::writeFile(char *filename, char *data, Mode mode)
{
    if(SIMcore.getCommandCounter() == 1){
        char num[4];
        int len = strlen(data);
        itoa(len, num, 10);
        if(mode == Normal){
            SIMcore.send_cmd("AT+FSWRITE=\"");
        }else if(mode == Hex){
            SIMcore.send_cmd("AT+FSWRITEHEX=\"");
        }else{
            return false;
        }
        SIMcore.send_cmd(filename);
        SIMcore.send_cmd("\",1,");
        SIMcore.send_cmd(num);
        if(SIMcore.check_send_cmd("\r\n",">")){
            if(SIMcore.check_send_cmd(data,"OK")){
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
}

bool  DFRobot_L218::createFolder(char *foldername)
{
    if(SIMcore.getCommandCounter() == 1){
        SIMcore.send_cmd("AT+FSMKDIR=\"");
        SIMcore.send_cmd(foldername);
        if(SIMcore.check_send_cmd("\"\r\n","OK")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool  DFRobot_L218::deleteFolder(char *foldername)
{
    if(SIMcore.getCommandCounter() == 1){
        SIMcore.send_cmd("AT+FSRMDIR=\"");
        SIMcore.send_cmd(foldername);
        if(SIMcore.check_send_cmd("\"\r\n","OK")){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool   DFRobot_L218::initPos(void)
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

bool   DFRobot_L218::getPosition(void)
{
    char  posBuffer[80];
    char *pLongitude,*pLatitude;
    SIMcore.cleanBuffer(posBuffer,80);
    if((SIMcore.getCommandCounter() == 2) || (SIMcore.getCommandCounter() == 5)){
//        delay(500);
        SIMcore.send_cmd("AT+GTPOS=2\r\n");
        SIMcore.readBuffer(posBuffer,80);
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
//Serial.println(posBuffer);
        pLongitude = strstr(posBuffer,":");
        pLatitude  = strstr(posBuffer,",");
        memcpy(longitude,pLongitude+2,7);
        memcpy(latitude, pLatitude+1 ,7);
        SIMcore.setCommandCounter(4);
        return true;
    }
}

char*   DFRobot_L218::getLongitude(void)
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
        return "Error";
    }
}

char*   DFRobot_L218::getLatitude(void)
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
        return "Error";
    }
}
