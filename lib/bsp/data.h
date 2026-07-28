#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

//================ 设备数据 =================
struct DeviceData
{
    // 传感器
    float temp = 0.0f;
    float humidity = 0.0f;
    float pressure = 0.0f;
    int   co2 = 0;

    bool airPower = true;
    int targetTemp = 24;
    uint8_t fanSpeed = 1;      // 0=LOW 1=MID 2=HIGH 3=AUTO
    uint8_t airSelect = 0;     // 0~2

    // UI
    int menuIndex = 0;
    int battery = 100;

    // WiFi
    bool wifiConnected = false;
    bool mqttConnected = false;
    WifiState wifiState ;

    String wifiList[10];
    int wifiCount = 0;
    int wifiSelectIndex = 0;

    // 保存的配置
    String savedSSID;
    String savedPWD;

    // 网络信息
    String ipAddress;
};

// 全局设备数据
extern DeviceData g_data;


//================ 配置保存 =================

// 开机读取配置
void loadConfig();

// 保存目标温度
void saveTemp(float temp);

// 保存 WiFi
void saveWifiConfig(String ssid, String pwd);


//================ wifi状态 =================
enum WifiState
{
    WIFI_LIST,
    WIFI_CONNECTING,
    WIFI_CONNECTED
};
#endif