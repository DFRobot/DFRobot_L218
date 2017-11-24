#include <DFRobot_L218file.h>

bool  DFRobot_L218file::init(void)
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

char* DFRobot_L218file::getList(void)
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

char* DFRobot_L218file::getList(char *foldername)
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

bool  DFRobot_L218file::createFile(char *filename)
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

char* DFRobot_L218file::readFile(char *filename, int offset, int len, Mode mode)
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

bool  DFRobot_L218file::deleteFile(char *filename)
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

bool  DFRobot_L218file::writeFile(char *filename, char *data, Mode mode)
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

bool  DFRobot_L218file::createFolder(char *foldername)
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

bool  DFRobot_L218file::deleteFolder(char *foldername)
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