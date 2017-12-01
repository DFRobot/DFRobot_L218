#include <DFRobot_L218.h>

bool  DFRobot_L218::initFile(void)
{
    if(!check_send_cmd("AT+FSCREATE=?\r\n","OK")){
        setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!check_send_cmd("AT+FSREAD=?\r\n","OK")){
        setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!check_send_cmd("AT+FSWRITE=?\r\n","OK")){
        setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!check_send_cmd("AT+FSMKDIR=?\r\n","OK")){
        setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!check_send_cmd("AT+FSRMDIR=?\r\n","OK")){
        setCommandCounter(0);
        return false;
    }
    delay(10);
    if(!check_send_cmd("AT+FSLS=?\r\n","OK")){
        setCommandCounter(0);
        return false;
    }
    setCommandCounter(1);
    return true;
}

char* DFRobot_L218::getList(void)
{
    if(getCommandCounter() == 1){
        char SIMList[300];
        cleanBuffer(SIMList,300);
        send_cmd("AT+FSLS\r\n");
        readBuffer(SIMList,300);
        return SIMList+9;
    }else{
        return "error";
    }
}

char* DFRobot_L218::getList(char *foldername)
{
    if(getCommandCounter() == 1){
        char SIMList[300];
        cleanBuffer(SIMList,300);
        send_cmd("AT+FSLS=\"");
        send_cmd(foldername);
        send_cmd("\"\r\n");
        readBuffer(SIMList,300);
        return SIMList+8;
    }else{
        return "error";
    }
}

char* DFRobot_L218::getSpace(void)
{
    if(getCommandCounter() == 1){
        char fileSpace[50];
        cleanBuffer(fileSpace,50);
        send_cmd("AT+FSINFO=\"Z:\"\r\n");
        readBuffer(fileSpace,50);
        return fileSpace+18;
    }else{
        return "error";
    }
}

bool  DFRobot_L218::createFile(char *filename)
{
    if(getCommandCounter() == 1){
        send_cmd("AT+FSCREATE=\"");
        send_cmd(filename);
        if(check_send_cmd("\"\r\n","OK")){
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
    if(getCommandCounter() == 1){
        int   rOffset = len+strlen(filename)+6;
        rOffset *=2;
        char  L218file[rOffset+50];
        cleanBuffer(L218file,rOffset+50);
        char  num1[4],num2[4];
        itoa(offset, num1, 10);
        itoa(len,    num2, 10);
        if(mode == Normal){
            send_cmd("AT+FSREAD=\"");
        }else if(mode == Hex){
            send_cmd("AT+FSREADHEX=\"");
        }else{
            return "No such mode";
        }
        send_cmd(filename);
        send_cmd("\",");
        send_cmd(num1);
        send_cmd(",");
        send_cmd(num2);
        send_cmd("\r\n");
        readBuffer(L218file,rOffset+50);
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
    if(getCommandCounter() == 1){
        send_cmd("AT+FSDEL=\"");
        send_cmd(filename);
        if(check_send_cmd("\"\r\n","OK")){
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
    if(getCommandCounter() == 1){
        char num[4];
        int len = strlen(data);
        itoa(len, num, 10);
        if(mode == Normal){
            send_cmd("AT+FSWRITE=\"");
        }else if(mode == Hex){
            send_cmd("AT+FSWRITEHEX=\"");
        }else{
            return false;
        }
        send_cmd(filename);
        send_cmd("\",1,");
        send_cmd(num);
        if(check_send_cmd("\r\n",">")){
            if(check_send_cmd(data,"OK")){
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
    if(getCommandCounter() == 1){
        send_cmd("AT+FSMKDIR=\"");
        send_cmd(foldername);
        if(check_send_cmd("\"\r\n","OK")){
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
    if(getCommandCounter() == 1){
        send_cmd("AT+FSRMDIR=\"");
        send_cmd(foldername);
        if(check_send_cmd("\"\r\n","OK")){
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
        if(check_send_cmd("AT+CGATT?\r\n","1")){
            break;
        }else{
            count++;
            delay(500);
        }
    }
    if(count == 3){
        closeCommand();
        return false;
    }
    if(check_send_cmd("AT+CSTT=\"CMNET\"\r\n","OK")){
        if(check_send_cmd("AT+GTPOS=1\r\n","CONNECT OK")){
            setCommandCounter(2);
            return true;
        }else{
            closeCommand();
            return false;
        }
    }else{
        closeCommand();
        return false;
    }
}

bool   DFRobot_L218::getPosition(void)
{
    char  posBuffer[80];
    char *pLongitude,*pLatitude;
    cleanBuffer(posBuffer,80);
    if((getCommandCounter() == 2) || (getCommandCounter() == 5)){
//        delay(500);
        send_cmd("AT+GTPOS=2\r\n");
        readBuffer(posBuffer,80);
        if(NULL != strstr(posBuffer,"+GTPOS")){
            if(NULL != strstr(posBuffer,"$")){
                setCommandCounter(3);
            }else if(NULL != strstr(posBuffer,"-1")){
                setCommandCounter(6);
            }else if(NULL != strstr(posBuffer,"-2")){
                setCommandCounter(7);
            }else if(NULL != strstr(posBuffer,"-3")){
                setCommandCounter(8);
            }else if(NULL != strstr(posBuffer,"-4")){
                setCommandCounter(9);
            }else if(NULL != strstr(posBuffer,"-5")){
                setCommandCounter(10);
            }else if(NULL != strstr(posBuffer,"-6")){
                setCommandCounter(11);
            }else if(NULL != strstr(posBuffer,"-7")){
                setCommandCounter(12);
            }
        }else{
            closeCommand();
            return false;
        }
    }else{
        closeCommand();
        return false;
    }
    if(getCommandCounter() == 3){
//Serial.println(posBuffer);
        pLongitude = strstr(posBuffer,":");
        pLatitude  = strstr(posBuffer,",");
        memcpy(longitude,pLongitude+2,7);
        memcpy(latitude, pLatitude+1 ,7);
        setCommandCounter(4);
        return true;
    }
}

char*   DFRobot_L218::getLongitude(void)
{
    if((getCommandCounter() == 4) || (getCommandCounter() == 5)){
        setCommandCounter(5);
        return longitude;
    }else if(getCommandCounter() == 6 ){
        setCommandCounter(2);
        return "Error:Network busy";
    }else if(getCommandCounter() == 7 ){
        setCommandCounter(2);
        return "Error:Not ready";
    }else if(getCommandCounter() == 8 ){
        setCommandCounter(2);
        return "Error:Network error";
    }else if(getCommandCounter() == 9 ){
        setCommandCounter(2);
        return "Error:Timeout";
    }else if(getCommandCounter() == 10){
        setCommandCounter(2);
        return "Error:Unack";
    }else if(getCommandCounter() == 11){
        setCommandCounter(2);
        return "Error:Network exists";
    }else if(getCommandCounter() == 12){
        setCommandCounter(2);
        return "Error:WIFI information error";
    }else{
        return "Error";
    }
}

char*   DFRobot_L218::getLatitude(void)
{
    if((getCommandCounter() == 4) || (getCommandCounter() == 5)){
        setCommandCounter(5);
        return latitude;
    }else if(getCommandCounter() == 6 ){
        setCommandCounter(2);
        return "Error:Network busy";
    }else if(getCommandCounter() == 7 ){
        setCommandCounter(2);
        return "Error:Not ready";
    }else if(getCommandCounter() == 8 ){
        setCommandCounter(2);
        return "Error:Network error";
    }else if(getCommandCounter() == 9 ){
        setCommandCounter(2);
        return "Error:Timeout";
    }else if(getCommandCounter() == 10){
        setCommandCounter(2);
        return "Error:Unack";
    }else if(getCommandCounter() == 11){
        setCommandCounter(2);
        return "Error:Network exists";
    }else if(getCommandCounter() == 12){
        setCommandCounter(2);
        return "Error:WIFI information error";
    }else{
        return "Error";
    }
}
