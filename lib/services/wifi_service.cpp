#include "wifi_service.h"

WifiService wifiService;

void WifiService::init()
{
    WiFi.mode(WIFI_AP_STA);

    queue_ = xQueueCreate(5, sizeof(WifiMessage));

    eventGroup_ = xEventGroupCreate();

    if(eventGroup_ == nullptr)
    {
        Serial.println("Create EventGroup Failed");
    }

}

void WifiService::update()
{
    WifiMessage msg;

    while(xQueueReceive(queue_, &msg, 0) == pdTRUE)//取队列
    {
        switch(msg.command)
        {
        case WifiCommand::Scan:
            scan();
            break;

        case WifiCommand::StartConfig:
            startConfig(msg.index);
            break;
        }
    }

    server_.handleClient();

    if(isConnecting())
    {
        if(WiFi.status() == WL_CONNECTED)
        {
            xEventGroupClearBits(eventGroup_,WIFI_CONNECTING_BIT);
            xEventGroupSetBits(eventGroup_,WIFI_CONNECTED_BIT);

            Serial.println("WiFi Connected!");
            Serial.println(WiFi.localIP());
        }
    }
}

void WifiService::scan()
{
    state_ = WifiState::Scanning;

    Serial.println("scan");

    vTaskDelay(pdMS_TO_TICKS(100));

    Serial.println("开始扫描");

    int n = WiFi.scanNetworks();

    if(n <= 0)
    {
        Serial.println("没有发现WiFi");
        g_data.wifiCount = 0;
        return;
    }

    g_data.wifiCount = min(n, 10);

    Serial.printf("发现 %d 个WiFi\n", g_data.wifiCount);

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

    state_ = WifiState::ListReady;

    WiFi.scanDelete();
}

bool WifiService::requestScan()
{
    WifiMessage msg;

    msg.command = WifiCommand::Scan;

    return xQueueSend(queue_, &msg, 0) == pdPASS;
}

void WifiService::startAP()
{

    WiFi.softAP(AP_ID, AP_PWD);

    apIP_ = WiFi.softAPIP().toString();

    Serial.println(apIP_);
}

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

    server_.send(200, "text/html", html);
}

void WifiService::handleSave()
{
    xEventGroupClearBits(eventGroup_,WIFI_CONNECTED_BIT); //取消打√
    xEventGroupSetBits(eventGroup_,WIFI_CONNECTING_BIT);//将这个事件打√

    WiFi.disconnect();

    selectedPWD_ = server_.arg("pwd");

    Serial.println("SSID:");
    Serial.println(selectedSSID_);

    Serial.println("PWD:");
    Serial.println(selectedPWD_);

    saveWifiConfig(selectedSSID_, selectedPWD_);

    g_data.savedSSID = selectedSSID_;
    g_data.savedPWD  = selectedPWD_;

    delay(100);
    
    state_ = WifiState::Connecting;
    WiFi.begin(
        selectedSSID_.c_str(),
        selectedPWD_.c_str());
    

    server_.send(200, "text/html", "<h1>Connecting...</h1>");
    state_ = WifiState::Connected;
}

void WifiService::startWebServer()
{
    if(serverStarted_)
        return;

    server_.on("/", [this]()
    {
        handleRoot();
    });

    server_.on("/save", [this]()
    {
        handleSave();
    });

    server_.begin();

    Serial.println("WebServer Started");

    serverStarted_=true;
}

void WifiService::setSelectedSSID(const String& ssid)
{
    selectedSSID_ = ssid;
}

bool WifiService::isConnecting() const
{
     EventBits_t bits = xEventGroupGetBits(eventGroup_);

    return bits & WIFI_CONNECTING_BIT;
}

bool WifiService::isConnected() const
{
    EventBits_t bits = xEventGroupGetBits(eventGroup_);

    return bits & WIFI_CONNECTED_BIT;
}

String WifiService::getSelectedSSID() const
{
    return selectedSSID_;
}

bool WifiService::startConfig(int index)
{
    if(index < 0 || index >= g_data.wifiCount)
        return false;

    selectedSSID_ = g_data.wifiList[index];

    startAP();
    state_ = WifiState::APStarted;

    startWebServer();

    return true;
}

bool WifiService::requestStartConfig(int index)
{
    WifiMessage msg;

    msg.command = WifiCommand::StartConfig;
    msg.index = index;

    return xQueueSend(queue_, &msg, 0) == pdPASS;
}

String WifiService::getApIP() const
{
    return apIP_;
}
