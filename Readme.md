# DFROBOT_L218 Library for Arduino
Provides an Arduino library for L218

## Table of Contents

* [Summary](#summary)
* [Methods](#methods)
* [History](#history)
* [Credits](#credits)
<snippet>
<content>

## Summary

The library use for L218 as following function

#### MPU6050 attitude sensor
#### Send short message
#### Get current position
#### SD card 
#### 

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
 * @brief Turn ON L218
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool tureON(void);

/*
 * @brief Check if L218 is turn on
 *
 * @return
 *     ture   Turn ON 
 *     false  Turn OFF
 */
bool check_TurnON(void);

/*
 * @brief Begin to send short message
 *
 * @param phoneNumber Target number for send short message
 * @return
 *     ture   Turn ON 
 *     false  Turn OFF
 */
bool beginSMS(const char* phoneNumber);

/*
 * @brief Send short message
 *
 * @param content The message content
 *
 * @return
 *     ture   Turn ON 
 *     false  Turn OFF
 */
bool sendSMS(const char* content);

/*
 * @brief Make a phone call
 *
 * @param phoneNumber Target number
 *
 * @return
 *     ture   Turn ON 
 *     false  Turn OFF
 */
bool voiceCall(const char* phoneNumber);

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
bool getPos(void);

/*
 * @brief Get longitude
 *
 * @return
 *     Longitude value
 */
double getLongitude(void);

/*
 * @brief Get latitude
 *
 * @return
 *     Latitude value
 */
double getLatitude(void);

/*
 * @brief Init internet
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool initNet(void);

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
 * @brief Enable MPU6050 attitude sensor
 *
 */
void startMPU6050(void);

/*
 * @brief Disable MPU6050 attitude sensor
 *
 */
void stopMPU6050(void);

/*
 * @brief LED blink
 *
 * @param times     How many times LED blink
 *        interval  LED blink interval
 */
void blink(int times , int interval);

/*
 * @brief L218 enter sleep mode
 *
 */
void sleepMode(void);

/*
 * @brief Wake up L218
 *
 */
void wakeUp(void);

```

## History

- data 2018-5-11
- version V1.0

## Credits

- author [Zhangjiawei  <jiawei.zhang@dfrobot.com>]