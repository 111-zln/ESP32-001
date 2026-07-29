#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include "data.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

enum class WifiCommand
{
    Scan,
    StartConfig
};

struct WifiMessage
{
    WifiCommand command;
    int index = -1;
};

class WifiService
{
public:
    void init();
    void update();

    // WiFi扫描
    void scan();
    bool requestScan();

    // 开热点
    void startAP();

    // Web配置
    void startWebServer();

    // 连接指定WiFi
    bool startConfig(int index);
    bool requestStartConfig(int index);

    // 当前选择的SSID
    void setSelectedSSID(const String& ssid);

    // 状态查询
    bool isConnecting() const;
    bool isConnected() const;

    //
    String getSelectedSSID() const;
    String getApIP() const;

    String apIP_;

    String AP_ID = "ESP32_Remoter" ;
    String AP_PWD = "12345678" ;

private:
    void handleRoot();
    void handleSave();

    WebServer server_{80};
    bool serverStarted_ = false;
    
    static constexpr EventBits_t WIFI_CONNECTING_BIT = BIT0;
    static constexpr EventBits_t WIFI_CONNECTED_BIT  = BIT1;

    String selectedSSID_;
    String selectedPWD_;

    //freertos
    QueueHandle_t queue_;
    EventGroupHandle_t eventGroup_;
};

extern WifiService wifiService;