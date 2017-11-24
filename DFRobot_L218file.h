#ifndef __DFRobot_L218FILE_H__
#define __DFRobot_L218FILE_H__

#include "DFRobot_SIMcore.h"

enum Mode{
    Normal,
    Hex,
};

class DFRobot_L218file
{
public:
    bool     init(void);
    char*    getList(void);
    char*    getList(char *foldername);
    bool     createFile(char *filename);
    char*    readFile(char *filename,int offset = 0, int len = 50, Mode mode = Normal);
    bool     deleteFile(char *filename);
    bool     writeFile(char *filename, char *data, Mode mode = Normal);
    bool     createFolder(char *foldername);
    bool     deleteFolder(char *foldername);
};

#endif