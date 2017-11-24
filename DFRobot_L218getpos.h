#ifndef __DFRobot_L218GETPOS_H__
#define __DFRobot_L218GETPOS_H__

#include "DFRobot_SIMcore.h"

class DFRobot_L218getpos
{
public:
    bool     init(void);
    bool     getPosition(void);
    char*    getLongitude(void);
    char*    getLatitude(void);
private:
    char     latitude[8];
    char     longitude[8];
};

#endif