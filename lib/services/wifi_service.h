#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include "data.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "board.h"

enum class WifiState
{
    Idle,        //空闲

    Scanning,    //扫描中

    ListReady,   //准备列表

    APStarted,   //AP已开启

    WaitingPassword,//等待获取密码

    Connecting,  //wifi连接中
  
    Connected,   //wifi连接成功

    Failed       //失败
};

//wifi指令
enum class WifiCommand 
{
    Scan,
    StartConfig,
    Retry  
};

//wifi信息
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

    //联网要求
    String getSelectedSSID() const;
    String getApIP() const;

    //AP信息
    String apIP_;
    String AP_ID = "ESP32_Remoter" ;
    String AP_PWD = "12345678" ;

    //获取wifi状态
    WifiState getState() const;

    //重连
    bool requestRetry();
    bool retry();

private:

    void handleRoot();
    void handleSave();

    void handleOTA(); 

    WebServer server_{80};
    bool serverStarted_ = false;
    
    static constexpr EventBits_t WIFI_CONNECTING_BIT = BIT0;
    static constexpr EventBits_t WIFI_CONNECTED_BIT  = BIT1;

    String selectedSSID_;
    String selectedPWD_;

    //freertos
    QueueHandle_t queue_;
    EventGroupHandle_t eventGroup_;

    
    WifiState state_ = WifiState::Idle; //wifi状态
    TickType_t connectStartTick_ = 0; //连接时间
    static constexpr TickType_t CONNECT_TIMEOUT =pdMS_TO_TICKS(15000); //连接超时时间 15s

};

extern WifiService wifiService;