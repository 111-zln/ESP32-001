#include "data.h"
#include <Preferences.h>

DeviceData g_data;

Preferences prefs;


//---------------- 配置读取 ----------------

void loadConfig()
{
    prefs.begin("config", true);

    g_data.temp = prefs.getFloat("temp", 25.0f);

    g_data.savedSSID = prefs.getString("ssid", "");
    g_data.savedPWD  = prefs.getString("pwd", "");

    prefs.end();
}

//---------------- 保存温度 ----------------

void saveTemp(float temp)
{
    prefs.begin("config", false);

    prefs.putFloat("temp", temp);

    prefs.end();
}

//---------------- 保存WiFi ----------------

void saveWifiConfig(String ssid, String pwd)
{
    prefs.begin("config", false);

    prefs.putString("ssid", ssid);
    prefs.putString("pwd", pwd);

    prefs.end();
}