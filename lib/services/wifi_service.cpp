#include "wifi_service.h"

WifiService wifiService;


void WifiService::init()
{
    WiFi.mode(WIFI_AP_STA);

    queue_ = xQueueCreate(5, sizeof(WifiMessage));

    eventGroup_ = xEventGroupCreate();

    if(queue_ == nullptr)
    {
        Serial.println("Create Queue Failed");
    }

    if(eventGroup_ == nullptr)
    {
        Serial.println("Create EventGroup Failed");
    }

    setState(WifiState::Idle);
}


void WifiService::update()
{
    WifiMessage msg;

    // =========================
    // 处理WiFi命令
    // =========================

    while(xQueueReceive(queue_, &msg, 0) == pdTRUE)
    {
        switch(msg.command)
        {
        case WifiCommand::Scan:

            scan();

            break;


        case WifiCommand::StartConfig:

            startConfig(msg.index);

            break;


        case WifiCommand::Retry:

            retry();

            break;
        }
    }


    // =========================
    // WebServer
    // =========================

    server_.handleClient();


    // =========================
    // WiFi连接状态检测
    // =========================

    if(state_ == WifiState::Connecting)
    {
        // 连接成功
        if(WiFi.status() == WL_CONNECTED)
        {
            setState(WifiState::Connected);

            Serial.println("WiFi Connected!");
            Serial.println(WiFi.localIP());

            board_.rgb_.green();

            vTaskDelay(pdMS_TO_TICKS(1000));

            board_.rgb_.off();
        }

        // 还没有连接成功
        else
        {
            TickType_t elapsed =
                xTaskGetTickCount() - connectStartTick_;


            // 超时
            if(elapsed > CONNECT_TIMEOUT)
            {
                WiFi.disconnect();

                setState(WifiState::Failed);

                Serial.println("WiFi Connect Timeout");


                // 红灯闪三次
                for(int i = 0; i < 3; i++)
                {
                    board_.rgb_.red();

                    vTaskDelay(pdMS_TO_TICKS(200));

                    board_.rgb_.off();

                    vTaskDelay(pdMS_TO_TICKS(200));
                }
            }
        }
    }
}


// WiFi扫描
// =====================================================
void WifiService::scan()
{
    setState(WifiState::Scanning);

    board_.rgb_.blue();

    Serial.println("scan");

    vTaskDelay(pdMS_TO_TICKS(100));

    Serial.println("开始扫描");


    int n = WiFi.scanNetworks();


    // 没有发现WiFi
    if(n <= 0)
    {
        Serial.println("没有发现WiFi");

        g_data.wifiCount = 0;

        setState(WifiState::ListReady);

        board_.rgb_.off();

        return;
    }


    // 最多保存10个WiFi
    g_data.wifiCount = min(n, 10);

    Serial.printf(
        "发现 %d 个WiFi\n",
        g_data.wifiCount
    );


    for(int i = 0; i < g_data.wifiCount; i++)
    {
        g_data.wifiList[i] = WiFi.SSID(i);

        Serial.print(i);
        Serial.print(": ");

        Serial.print(g_data.wifiList[i]);

        Serial.print(" (");

        Serial.print(WiFi.RSSI(i));

        Serial.println(" dBm)");
    }


    setState(WifiState::ListReady);

    WiFi.scanDelete();

    board_.rgb_.off();
}



// 请求扫描
// =====================================================
bool WifiService::requestScan()
{
    WifiMessage msg;

    msg.command = WifiCommand::Scan;

    return xQueueSend(
        queue_,
        &msg,
        0
    ) == pdPASS;
}


// 开启AP
// =====================================================
void WifiService::startAP()
{
    WiFi.softAP(
        AP_ID,
        AP_PWD
    );

    apIP_ = WiFi.softAPIP().toString();

    Serial.println(apIP_);
}


// Web根页面
// =====================================================
void WifiService::handleRoot()
{
    String html;

    html += "<html><body>";

    html += "<h2>ESP32 WiFi Config</h2>";

    html += "<h3>";
    html += selectedSSID_;
    html += "</h3>";

    html += "<form action='/save'>";

    html += "Password:<br>";

    html += "<input name='pwd'><br><br>";

    html += "<input type='submit' value='Connect'>";

    html += "</form>";

    html += "</body></html>";


    server_.send(
        200,
        "text/html",
        html
    );
}


// 保存WiFi密码并开始连接
// =====================================================
void WifiService::handleSave()
{
    WiFi.disconnect();

    selectedPWD_ = server_.arg("pwd");


    Serial.println("SSID:");
    Serial.println(selectedSSID_);

    Serial.println("PWD:");
    Serial.println(selectedPWD_);


    // 保存配置
    saveWifiConfig(
        selectedSSID_,
        selectedPWD_
    );


    g_data.savedSSID = selectedSSID_;
    g_data.savedPWD  = selectedPWD_;


    vTaskDelay(
        pdMS_TO_TICKS(100)
    );


    // 开始连接
    setState(WifiState::Connecting);

    connectStartTick_ =
        xTaskGetTickCount();


    WiFi.begin(
        selectedSSID_.c_str(),
        selectedPWD_.c_str()
    );


    server_.send(
        200,
        "text/html",
        "<h1>Connecting...</h1>"
    );
}


// 启动WebServer
// =====================================================
void WifiService::startWebServer()
{
    if(serverStarted_)
        return;


    // =========================
    // WiFi配置首页
    // =========================

    server_.on(
        "/",
        [this]()
        {
            handleRoot();
        }
    );


    // =========================
    // 保存WiFi密码
    // =========================

    server_.on(
        "/save",
        [this]()
        {
            handleSave();
        }
    );


    // =========================
    // OTA页面
    // =========================

    server_.on(
        "/update",
        HTTP_GET,
        [this]()
        {
            handleOTA();
        }
    );


    // =========================
    // OTA上传
    // =========================

    server_.on(
        "/update",
        HTTP_POST,

        [this]()
        {
            server_.send(
                200,
                "text/plain",
                "Update Success, Rebooting..."
            );
        },

        [this]()
        {
            handleOTAUpload();
        }
    );


    server_.begin();

    Serial.println("WebServer Started");

    serverStarted_ = true;
}


// 设置当前选择的SSID
// =====================================================
void WifiService::setSelectedSSID(
    const String& ssid
)
{
    selectedSSID_ = ssid;
}


// 判断是否正在连接
// =====================================================
bool WifiService::isConnecting() const
{
    EventBits_t bits =
        xEventGroupGetBits(eventGroup_);

    return bits & WIFI_CONNECTING_BIT;
}



// 判断是否已经连接
// =====================================================
bool WifiService::isConnected() const
{
    EventBits_t bits =
        xEventGroupGetBits(eventGroup_);

    return bits & WIFI_CONNECTED_BIT;
}


// 获取WiFi状态
// =====================================================
WifiState WifiService::getState() const
{
    return state_;
}


// 获取SSID
// =====================================================
String WifiService::getSelectedSSID() const
{
    return selectedSSID_;
}


// 开始WiFi配置
// =====================================================
bool WifiService::startConfig(int index)
{
    if(index < 0 || index >= g_data.wifiCount)
        return false;


    selectedSSID_ =
        g_data.wifiList[index];


    startAP();

    setState(WifiState::APStarted);


    startWebServer();

    setState(WifiState::WaitingPassword);


    return true;
}


// 请求开始配置
// =====================================================
bool WifiService::requestStartConfig(int index)
{
    WifiMessage msg;

    msg.command =
        WifiCommand::StartConfig;

    msg.index = index;


    return xQueueSend(
        queue_,
        &msg,
        0
    ) == pdPASS;
}


// 获取AP IP
// =====================================================
String WifiService::getApIP() const
{
    return apIP_;
}


// 请求Retry
// =====================================================
bool WifiService::requestRetry()
{
    WifiMessage msg;

    msg.command =
        WifiCommand::Retry;


    return xQueueSend(
        queue_,
        &msg,
        0
    ) == pdPASS;
}


// Retry
// =====================================================
bool WifiService::retry()
{
    if(state_ != WifiState::Failed)
        return false;


    WiFi.disconnect();

    vTaskDelay(
        pdMS_TO_TICKS(100)
    );


    connectStartTick_ =
        xTaskGetTickCount();


    setState(WifiState::Connecting);


    WiFi.begin(
        selectedSSID_.c_str(),
        selectedPWD_.c_str()
    );


    Serial.println("WiFi Retry");


    return true;
}


// OTA页面
// =====================================================
void WifiService::handleOTA()
{
    String html;

    html += "<html><body>";

    html += "<h2>ESP32 OTA</h2>";

    html +=
        "<form method='POST' "
        "action='/update' "
        "enctype='multipart/form-data'>";

    html +=
        "<input type='file' "
        "name='firmware'>"
        "<br><br>";

    html +=
        "<input type='submit' "
        "value='Upload'>";

    html += "</form>";

    html += "</body></html>";


    server_.send(
        200,
        "text/html",
        html
    );
}


// OTA文件上传
// =====================================================
void WifiService::handleOTAUpload()
{
    HTTPUpload& upload =
        server_.upload();


    switch(upload.status)
    {
    // =========================
    // 文件开始
    // =========================

    case UPLOAD_FILE_START:
    {
        Serial.println("Upload Start");

        setState(WifiState::OTAUpdating);


        if(!Update.begin(UPDATE_SIZE_UNKNOWN))
        {
            Serial.println("OTA Begin Failed");

            Update.printError(Serial);

            setState(WifiState::OTAFailed);
        }

        break;
    }


    // =========================
    // 文件数据
    // =========================

    case UPLOAD_FILE_WRITE:
    {
        // 如果前面已经失败
        // 就不要继续写
        if(state_ == WifiState::OTAFailed)
            break;


        if(
            Update.write(
                upload.buf,
                upload.currentSize
            )
            != upload.currentSize
        )
        {
            Serial.println("OTA Write Failed");

            Update.printError(Serial);

            setState(WifiState::OTAFailed);
        }

        break;
    }


    // =========================
    // 文件结束
    // =========================

    case UPLOAD_FILE_END:
    {
        if(state_ == WifiState::OTAFailed)
        {
            Update.abort();

            break;
        }


        if(Update.end(true))
        {
            setState(WifiState::OTASuccess);

            Serial.println("OTA Success");

            Serial.println("Reboot...");

            ESP.restart();
        }
        else
        {
            Serial.println("OTA End Failed");

            Update.printError(Serial);

            setState(WifiState::OTAFailed);
        }

        break;
    }


    default:

        break;
    }
}


// 统一设置WiFi状态
// =====================================================
void WifiService::setState(WifiState state)
{
    state_ = state;


    if(eventGroup_ == nullptr)
        return;


    // 先清除两个WiFi状态
    xEventGroupClearBits(
        eventGroup_,
        WIFI_CONNECTING_BIT |
        WIFI_CONNECTED_BIT
    );


    // Connecting
    if(state_ == WifiState::Connecting)
    {
        xEventGroupSetBits(
            eventGroup_,
            WIFI_CONNECTING_BIT
        );
    }


    // Connected
    else if(state_ == WifiState::Connected)
    {
        xEventGroupSetBits(
            eventGroup_,
            WIFI_CONNECTED_BIT
        );
    }
}