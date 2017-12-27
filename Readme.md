# DFROBOT_L218 Library for Arduino
Provides an Arduino library for L218

## Table of Contents

* [Summary](#summary)
* [Methods](#methods)
* [Depends](#depends)
* [History](#history)
* [Credits](#credits)
<snippet>
<content>

## Summary

The library use for L218 as following function

#### Get time
#### Get current position
#### Independent file system

## Methods
```C++

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

/*
 * @brief MQTT connect request
 *
 * @param iot_client Client name user-defined
 *
 * @param iot_username The user name identifies the name of the user who is connecting
 *
 * @param iot_key The password for user
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool MQTTconnect(char* iot_client, char* iot_username, char* iot_key);

/*
 * @brief MQTT send command
 *
 * @param iot_topic Target topic
 *
 * @param iot_data  The data you want to send
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool MQTTsend(char* iot_topic, char* iot_data);

/*
 * @brief MQTT subscribe request
 *
 * @param iot_topic The topic you want to subscribe
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool MQTTsubscribe(char* iot_topic);

/*
 * @brief Receive data from subscribed topic
 *
 * @param iot_topic The subscribed topic
 *
 * @param buff      The buffer to store data
 *
 * @param maxlen    The maximum length of the buffer
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool     MQTTrecv(char* iot_topic, char* buff,int maxlen);

/*
 * @brief MQTT unsubscribe request
 *
 * @param iot_topic The topic you want to unsubscribe
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool MQTTunsubscribe(char* iot_topic);

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