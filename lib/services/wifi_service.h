#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include "data.h"

class WifiService
{
public:
    void init();
    void update();

    // WiFi扫描
    void scan();

    // 开热点
    void startAP();

    // Web配置
    void startWebServer();

    // 连接指定WiFi
    bool startConfig(int index);

    // 当前选择的SSID
    void setSelectedSSID(const String& ssid);

    // 状态查询
    bool isConnecting() const;
    bool isConnected() const;

    String getSelectedSSID() const;

    String getApIP() const;

    String apIP_;

    String AP_ID = "ESP32_Remoter" ;
    String AP_PWD = "12345678" ;

private:
    void handleRoot();
    void handleSave();

    WebServer server_{80};

    bool wifiConnecting_ = false;
    bool wifiConnected_ = false;
    bool serverStarted_ = false;

    String selectedSSID_;
    String selectedPWD_;
};

extern WifiService wifiService;