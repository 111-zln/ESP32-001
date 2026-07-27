#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include "../bsp/data.h"

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

    // 当前选择的SSID
    void setSelectedSSID(const String& ssid);

    // 状态查询
    bool isConnecting() const;
    bool isConnected() const;

    String getSelectedSSID() const;

private:
    void handleRoot();
    void handleSave();

    WebServer server_{80};

    bool wifiConnecting_ = false;
    bool wifiConnected_ = false;

    String selectedSSID_;
    String selectedPWD_;
};

extern WifiService wifiService;