/*
 QiaqiaMQTT.h - the MQTT code for qiaqia cloud platform connection.
  Qiaqia Cloud
*/
#ifndef QiaqiaMqttSingleDevice_h
#define QiaqiaMqttSingleDevice_h

#include <Arduino.h>
#include "Client.h"
#include <PubSubClient.h>
#include <SPI.h>
#include <WiFi.h>
#include <MsgHandler.h>

#include "SLFS.h"  //zdf  文件系统头文件
#include <prcm.h>  //zdf   软复位&RTC
#include "HttpClient.h"  //zdf  

#define DATA_FUN_POINT 1000
#define EPINDEX "cc3200"
#define EQUAL 0

#define AP_NAME  "JT_Device"   
#define AP_PASSWORD  "bjjttecdev"

//board button 
#define button_SW2  PUSH2
#define ledPin  GREEN_LED

//This enables lots of debug output.
#define DEVELOP_VERSION
#ifdef DEVELOP_VERSION
#define SINGLEDEVICE_DEBUG
#endif

#ifdef SINGLEDEVICE_DEBUG
#define SD_DBG_ln Serial.println
#define SD_DBG Serial.print
#else 
#define SD_DBG_ln
#define SD_DBG
#endif

typedef void(*readcallback)(int offsetindex, char *ackdev);   //定义读回调函数类型
typedef void(*writecallback)(int offsetindex, float value);   //定义写回调函数类型

/*没使用AP config版本
typedef struct SGateWay
{
	char *epindex;
	char *sdindex;
	char *apikey;
	char userid[25];
	bool sd_sub_successed;
	char cloudip[20];
	char cloudport[6];
} gateWay;
*/

typedef struct {
	char *epindex;
	char userid[15];
	bool sd_sub_successed;

	char sdindex[25];
	char apikey[30];
    char cloudip[20];
    char cloudport[6];
    char ssid[30];
    char password[30];
} singleDeviceInfo;


/*****存储解析后的topic信息*****/
typedef struct 
{
	char str_from[15];
	char str_ver[15];
	char str_srcuserid[25];
	char str_sflag[25];
	char str_epindex[25];
	char str_devindex[25];
	char str_subindex[25];
	char str_userid[25];
	char str_offsetindex[15];
	char str_cmd[15];
} topicParseInfo;

/*****存储server读本地时的payload信息*****/
typedef struct 
{
	char offsetindex[15];
	char devack[80];
} sdReadCallbackParameters;

/*****存储server写本地时的payload信息*****/
typedef struct 
{
	//char busport[15];
	//char devaddr[15];
	char offsetindex[15];
	char value[25];
} sdWriteCallbackParameters;

/*****本地设备响应server读命令回传时所需要的信息*****/
typedef struct 
{
	char offsetindex_src[15];
	char userid_des[25];
	char epindex_des[25];
	char devindex_des[25];
	char offsetindex_des[15];
} sdAckInfo;

/*****OTA升级从server收到的升级路径信息*****/
typedef struct   //zdf
{
	//char updateHeader[10];   //使用第一版升级程序时需要这个成员变量
	char updateIP[15];
	char updatePort[15];
	char updatePath[100];
} sdUpdateParameters;


class QiaqiaMQTTSD {
private:
	String _sdindex;
	const char *_ackuseridcmd;   //存储topic中的命令类型
	char _cloudip[20];
	char _cloudport[6];
	String _acksdpayload[4];    //响应读命令时上传的payload信息
	String _valuechangepayload[1];

	uint8_t readFileFlag;  //用于标记上电后第一次读flash file。
	char *apName;
	char *apPassword;
	uint8_t oldCountClients;
	uint8_t countClients;

	unsigned long apConfigStart;
	uint8_t apFirstTimeState;
	
	uint8_t RemoteFirstTimeState;
	unsigned long RemoteConfigStart;

	//button
	uint8_t button_first_state;
  	unsigned long button_timer_begin;

	void sdDataMemoryInit();	
	int sdAckuseridSub();   //设备向云端订阅
	int sdAckuseridRegist();   //设备注册
	void sdRWSub();  //设备订阅读写

	void sdPayloadAckuseridParse(byte *payload, unsigned int length);    //解析注册时云端传下来的payload的
	sdReadCallbackParameters sdReadCallbackParametersDecode(byte *payload, unsigned int length);
	sdWriteCallbackParameters sdWriteCallbackParametersDecode(byte *payload, unsigned int length);
	sdAckInfo sdAckDecode(char *ackdev);

	void sdUpdateParametersDecode(byte *payload, unsigned int length); //zdf
	int sdUpdateFirmware(char* cloudip, char* port, char* path); //zdf



public:
	QiaqiaMQTTSD();

	singleDeviceInfo m_SdInfo;
	topicParseInfo m_topicParseInfo;
	sdReadCallbackParameters m_sdReadCallbackParameters;
	sdWriteCallbackParameters m_sdWriteCallbackParameters;
	sdAckInfo m_sdAckInfo;

	WiFiClient wifiClient;
	MsgHandler *pMsgHandler;
	PubSubClient *pClient;

	sdUpdateParameters m_sdUpdateParameters;  //zdf
	WiFiClient httpClient;  //zdf

	void sdMQTTDataParse(char *topic, byte *payload, unsigned int length);
	void sdTopicParse(char *topic);

	//void gwInit(char *sdindex, char *apikey, const char *cloudip);
	void sdInit();
	bool sdRun();
	//int online(int busport, int devaddr, bool onlinestate);
	int sdValueChange(int offsetindex, String value);
	int sdAck(char *ackdev, String value);

	void setReadCallback(readcallback outerreadcall);
	readcallback m_ReadCallback;
	void setWriteCallback(writecallback outerwritecall);
	writecallback m_WriteCallback;
	//void setGwAndDevTimeCallback(settimecallback outersettimecall);
	//settimecallback m_SetTimeCallback;
	//void setGwAndDevRestartCallback(setrestartcallback outersetrestartcall);
	//setrestartcallback m_SetRestartCallback;


	//void gwWiFiClear();  //zdf
	bool sdWifiConfig(void);  //zdf
	int sdUpdateStatus(String value);  //zdf 1111

	//void gwRTCInit();  //zdf 设备RTC初始化
	//time_t strTime2unix(char* timeStamp);  //typedef long time_t;
	//void unixTime2Str(int n, char* strTime, int bufLen);

	void sdAPModeConfig(void);
	//void gwSTAModeConfig(void);

	//void buttonAPTest(void);

};
#endif