#include "wifi_service.h"

WifiService wifiService;

void WifiService::init()
{
    WiFi.mode(WIFI_AP_STA);

    wifiConnecting_ = false;
    wifiConnected_ = false;
}

void WifiService::update()
{
    server_.handleClient();

    if(wifiConnecting_)
    {
        if(WiFi.status() == WL_CONNECTED)
        {
            wifiConnecting_ = false;
            wifiConnected_  = true;

            g_data.wifiConnected = true;

            Serial.println("WiFi Connected!");
            Serial.println(WiFi.localIP());
        }
    }
}

void WifiService::scan()
{
    Serial.println("scan");

    WiFi.mode(WIFI_AP_STA);

    delay(100);

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

    WiFi.scanDelete();
}

void WifiService::startAP()
{
    WiFi.mode(WIFI_AP_STA);

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
    wifiConnecting_ = true;
    wifiConnected_ = false;

    selectedPWD_ = server_.arg("pwd");

    Serial.println("SSID:");
    Serial.println(selectedSSID_);

    Serial.println("PWD:");
    Serial.println(selectedPWD_);

    saveWifiConfig(selectedSSID_, selectedPWD_);

    g_data.savedSSID = selectedSSID_;
    g_data.savedPWD  = selectedPWD_;

    WiFi.begin(
        selectedSSID_.c_str(),
        selectedPWD_.c_str());

    server_.send(200, "text/html", "<h1>Connecting...</h1>");
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
    return wifiConnecting_;
}

bool WifiService::isConnected() const
{
    return wifiConnected_;
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

    startWebServer();

    return true;
}

String WifiService::getApIP() const
{
    return apIP_;
}
