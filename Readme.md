# DFROBOT_L218 Library for Arduino
- Provides an Arduino library for L218
- L218 is a excellent global positioning navigation chip rely on GPRS+GPS+BDS(BeiDou Navigation Satellite System), it a Low-power High-performance module 
supports Quad-band GSM/GPRS((850/900/1800/1900)). The chip has particularly wide operating temperature and internal integration of the TCP / IP protocol with 
small package.

## Table of Contents

* [Summary](#summary)
* [Methods](#methods)
* [Depends](#depends)
* [History](#history)
* [Credits](#credits)
<snippet>
<content>

## Summary

### The library provide some application rely on L218:

#### Make a phone call
#### Send short massage
#### Get time
#### Get current position
#### Independent file system

## Methods
```C++

/*
 * @brief Set software serial
 *
 * @Param
 *     &s_ SoftwareSerial
 */
void begin(Stream &s_);

/*
 * @brief Check signal quality
 *
 * @return
 *     0-30:Signal quality
 */
int checkSignalQuality(void);

/*
 * @brief Start up connection
 *
 * @param 
 *    *server Server domain name
 *     ptl    Choose connection protocol Passible value: TCP UDP
 *     port   Contented port
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool connect(char *server,Protocol ptl,int port);

/*
 * @brief Send data
 *
 * @param 
 *    *data The data to send
 */
void send(char *data);

/*
 * @brief Send data with specify the length
 *
 * @param 
 *     buffer The buffer stored data to be send
 *     len    The length of data to be send
 */
void send(void* buffer,size_t len);

/*
 * @brief End the connection
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool close(void);

/*
 * @brief Get time
 *
 * @return
 *     Current time
 */
char* getTime(void);

/*
 * @brief Start a voice call
 * 
 * @param
 *     number Target phonenumber
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool voiceCall(char* number);

/*
 * @brief Hang up the call
 */
void hangCall(void);

/*
 * @brief Ready to send short message
 *
 * @param 
 *     to     Target number for send short message
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool beginSMS(const char* to);

/*
 * @brief Edit message content
 *
 * @param 
 *     c   The message content
 */
void editSMS(const char* c);

/*
 * @brief Send short message
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool sendSMS(void);

/*
 * @brief Get file list
 *
 * @return
 *     The list of all file
 */

/*
 * @brief Init L218 module
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool init(void);

/*
 * @brief Init L218 positioning module
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool initPos(void);

/*
 * @brief Get the current position
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool getPosition(void);

/*
 * @brief Get longitude
 *
 * @return
 *     Longitude value
 */
char* getLongitude(void);

/*
 * @brief Get latitude
 *
 * @return
 *     Latitude value
 */
char* getLatitude(void);

/*
 * @brief Init L218 file system
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool initFile(void);

/*
 * @brief Get file list
 *
 * @return
 *     The list of all file
 */
char* getList(void);

/*
 * @brief Get specified folder's list
 *
 * @param 
 *    *foldername   The name of specified folder
 *
 * @return
 *     The list of specified folder
 */
char* getList(char *foldername);

/*
 * @brief Get disk space information
 *
 * @return
 *     The size of disk space
 */
char* getSpace(void);

/*
 * @brief Create a new file
 *
 * @param 
 *    *filename   The name of new file
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool createFile(char *filename);

/*
 * @brief Create a new folder
 *
 * @param 
 *    *foldername   The name of new folder
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool createFolder(char *foldername);

/*
 * @brief Read a file data
 *
 * @param 
 *     *filename   The name of specified file
 *      offset     Offset from the file beginning Passible value: 0-65536, default value: 0
 *      len        The length of data to be read Passible value: 0-1024, default value: 50
 *      mode       Read mode Passible value: Normal Hex, default value: Normal
 *
 * @return
 *     The data of specified file
 */
char* readFile(char *filename, int offset, int len, Mode mode);

/*
 * @brief Delete specified file
 *
 * @param
 *    *filename   The name of specified file
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool deleteFile(char *filename);

/*
 * @brief Delete specified folder
 *
 * @param
 *    *folder   The name of specified folder
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool deleteFolder(char *foldername);

/*
 * @brief Write data to specified file
 *
 * @note  Folder must be empty
 *
 * @param
 *    *filename   The name of specified file
 *    *data       The input data
 *     mode       Write mode Passible value :Normal Hex, default value: Normal
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool writeFile(char *filename, char *data, Mode mode);

```

## Depends

Projects           |                     URL                       | Remarks
------------------ | :-------------------------------------------: | -----------
DFRobot_SIM        |  https://github.com/DFRobot/DFRobot_SIM       | must

## History

- data 2017-11-22
- version V0.1

## Credits

- author [Zhangjiawei  <jiawei.zhang@dfrobot.com>]