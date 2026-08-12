#pragma once

#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>

#include "data.h"
#include "board.h"

#include "freertos/queue.h"
#include "freertos/event_groups.h"


// =====================================================
// WiFi 状态
// =====================================================

enum class WifiState
{
    Idle,               // 空闲

    Scanning,           // 扫描中
    ListReady,          // WiFi列表准备完成

    APStarted,          // AP已开启
    WaitingPassword,    // 等待网页输入密码

    Connecting,         // WiFi连接中
    Connected,          // WiFi连接成功
    Failed,             // WiFi连接失败

    OTAReady,           // 可以进行OTA
    OTAUpdating,        // OTA升级中
    OTASuccess,         // OTA升级成功
    OTAFailed           // OTA升级失败
};


// =====================================================
// WiFi 指令
// =====================================================

enum class WifiCommand
{
    Scan,
    StartConfig,
    Retry
};


// =====================================================
// WiFi 消息
// =====================================================

struct WifiMessage
{
    WifiCommand command;

    int index = -1;
};


// =====================================================
// WiFi Service
// =====================================================

class WifiService
{
public:

    // =========================
    // 生命周期
    // =========================

    void init();
    void update();


    // =========================
    // WiFi 扫描
    // =========================

    void scan();
    bool requestScan();


    // =========================
    // AP
    // =========================

    void startAP();


    // =========================
    // Web 配置
    // =========================

    void startWebServer();

    bool startConfig(int index);
    bool requestStartConfig(int index);


    // =========================
    // Retry
    // =========================

    bool retry();
    bool requestRetry();


    // =========================
    // SSID
    // =========================

    void setSelectedSSID(const String& ssid);

    String getSelectedSSID() const;


    // =========================
    // WiFi 状态
    // =========================

    WifiState getState() const;

    bool isConnecting() const;
    bool isConnected() const;


    // =========================
    // AP / 网络信息
    // =========================

    String getApIP() const;


    // =========================
    // AP 配置
    // =========================

    static constexpr const char* AP_ID  = "ESP32_Remoter";
    static constexpr const char* AP_PWD = "12345678";


private:

    // =================================================
    // WebServer
    // =================================================

    void handleRoot();
    void handleSave();


    // =================================================
    // OTA
    // =================================================

    void handleOTA();
    void handleOTAUpload();


    // =================================================
    // 状态管理
    // =================================================

    void setState(WifiState state);


    // =================================================
    // WebServer
    // =================================================

    WebServer server_{80};

    bool serverStarted_ = false;


    // =================================================
    // EventGroup
    // =================================================

    static constexpr EventBits_t WIFI_CONNECTING_BIT = BIT0;
    static constexpr EventBits_t WIFI_CONNECTED_BIT  = BIT1;


    // =================================================
    // 当前 WiFi
    // =================================================

    String selectedSSID_;
    String selectedPWD_;


    // =================================================
    // AP 信息
    // =================================================

    String apIP_;


    // =================================================
    // FreeRTOS
    // =================================================

    QueueHandle_t queue_ = nullptr;

    EventGroupHandle_t eventGroup_ = nullptr;


    // =================================================
    // WiFi 状态
    // =================================================

    WifiState state_ = WifiState::Idle;


    // =================================================
    // WiFi 连接超时
    // =================================================

    TickType_t connectStartTick_ = 0;

    static constexpr TickType_t CONNECT_TIMEOUT =
        pdMS_TO_TICKS(15000);
};


// =====================================================
// 全局 WifiService
// =====================================================

extern WifiService wifiService;