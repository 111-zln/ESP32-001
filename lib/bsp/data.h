#ifndef DATA_H
#define DATA_H
#include <Arduino.h>

struct DeviceData
{
    int temp;

    int battery;

    bool wifiConnected;

    bool mqttConnected;

    String wifiList[10];

    int wifiCount;

    int wifiSelectIndex;

    String savedSSID;
    String savedPWD;
};

//保存
void loadConfig();//开机读取配置

void saveTemp(int temp);//保存温度

void saveWifiConfig(String ssid,String pwd);



extern DeviceData g_data;
extern int key_num ;//短按次数

//wifi名称和密码
extern String selectedSSID;
extern String selectedPWD;

extern bool wifiScanning ;//wifi是否在扫描
extern bool displaySleep ;//屏幕是否熄灭
extern bool wifiNeedScan ;//是否需要扫描wifi
extern unsigned long lastOperateTime ;
extern bool needRefresh;//页面是否需要刷新

extern int wifi_maxnum ;

extern bool wifiConnecting;  
extern bool wifiConnected;
extern String ipAddress;

#endif

