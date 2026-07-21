#include "data.h"
#include <Preferences.h>

DeviceData g_data;

int key_num =0 ;

bool wifiScanning = false;

bool needRefresh = true;

bool displaySleep = false;

bool wifiNeedScan = true;

unsigned long lastOperateTime = 0;


String selectedSSID = "";
String selectedPWD = "";

int wifi_maxnum = 5 ;
bool wifiConnecting = false;
bool wifiConnected = false;
String ipAddress = "";

Preferences prefs;

void loadConfig()//开机读取配置
{
    prefs.begin("config", true);//打开名为 config 的存储区，只能读

    g_data.temp =prefs.getInt("temp", 25); //找 temp,如果找到,返回保存值;如果没找到,返回25

    g_data.savedSSID = prefs.getString("ssid","");
    g_data.savedPWD  = prefs.getString("pwd","");

    prefs.end();
}

void saveTemp(int temp)
{
    prefs.begin("config", false);//打开存储区，可读可写

    prefs.putInt("temp", temp);//存入修改后的当前温度

    prefs.end();
}

void saveWifiConfig(String ssid,String pwd)
{
    prefs.begin("config",false);

    prefs.putString("ssid",ssid);

    prefs.putString("pwd",pwd);

    prefs.end();
}