# DFROBOT_L218 Library for Arduino
Provides an Arduino library for L218
  L218 is a excellent global positioning navigation chip rely on GPRS+GPS+BDS(BeiDou Navigation Satellite System), it a Low-power High-performance module 
supports Quad-band GSM/GPRS((850/900/1800/1900)). The chip has particularly wide operating temperature and internal integration of the TCP / IP protocol with 
small package.

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
#### Send data to iot
#### SD card
#### RTC
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
bool turnON(void);

/*
 * @brief Check if L218 is turn on
 *
 * @return
 *     ture   Turn ON 
 *     false  Turn OFF
 */
bool checkTurnON(void);

/*
 * @brief Turn OFF L218
 *
 */
void turnOFF(void);

/*
 * @brief Check if SIM card is inserted
 *
 * @return
 *     ture   Card inserted
 *     false  No card
 */
bool checkSIMcard(void);

/*
 * @brief Check battery power
 *
 * @return
 *     Battery power value(possible value: 0,10,20,30,40,50,60,70,80,90,100)
 *
 */
int  checkBattery(void);

/*
 * @brief Begin to send short message
 *
 * @param phoneNumber Target number for send short message
 *
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
bool initNetwork(void);

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
bool mqttConnect(char* iot_client, char* iot_username, char* iot_key);

/*
 * @brief MQTT publish command
 *
 * @param iot_topic Target topic
 *
 * @param iot_data  The data you want to publish
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool mqttPublish(char* iot_topic, String iot_data);

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

/*
 * @brief Initialize HTTP service
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool httpInit(void);

/*
 * @brief HTTP POST
 *
 * @param URL  POST URL
 *        data POST data
 *
 * @return
 *     ture   Success
 *     false  Failed
 */
bool httpPost(const char *URL, String data);

/*
 * @brief HTTP GET
 *
 * @param URL  GET URL
 *
 * @Note This function print the get data
 */
void httpGet(void);

/*
 * @brief Disconnect from server and cancel initialization
 *
 */
void httpDisconnect(void);

```

## History

- data 2018-5-11
- version V1.0

## Credits

- author [Zhangjiawei  <jiawei.zhang@dfrobot.com>]