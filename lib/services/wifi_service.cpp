#include <WiFi.h>
#include "wifi_service.h"

void wifi_loop()
{}

void scanWifi()
{
    Serial.println("scan");

    WiFi.mode(WIFI_AP_STA);

    delay(100);

    wifiScanning = true;          // 通知后台：我正在扫描

    Serial.println("开始扫描");

    int n = WiFi.scanNetworks();

    wifiScanning = false;     // 扫描结束

    if(n <= 0)
    {
        Serial.println("没有发现WiFi");

        g_data.wifiCount = 0;



        return;
    }

    g_data.wifiCount = min(n,10);

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

    wifiScanning = false;        // 扫描结束
}

//手机连接esp32热点，为了输入WiFi：id/password
void startAP()
{
    WiFi.mode(WIFI_AP_STA);//wifi设为ap-sta模式，打开热点，可让手机连接热点

    WiFi.softAP("ESP32_Remoter","12345678");

    Serial.println(WiFi.softAPIP());
}

WebServer server(80);
void handleRoot()
{
    String html;

    html += "<html>";
    html += "<body>";

    html += "<h2>ESP32 WiFi Config</h2>";

    html += "<h2>";
    html += selectedSSID;
    html += "</h2>";

    html += "<form action='/save'>";

    html += "Password:<br>";
    html += "<input name='pwd'><br><br>";

    html += "<input type='submit' value='Connect'>";

    html += "</form>";

    html += "</body>";
    html += "</html>";

    server.send(200,"text/html",html);
}

void handleSave()
{
    wifiConnecting = true;
    wifiConnected = false;

    selectedPWD = server.arg("pwd");

    Serial.println("SSID:");
    Serial.println(selectedSSID);

    Serial.println("PWD:");
    Serial.println(selectedPWD);

    saveWifiConfig(selectedSSID,selectedPWD);

    g_data.savedSSID = selectedSSID;
    g_data.savedPWD = selectedPWD;



    WiFi.begin(
        selectedSSID.c_str(),
        selectedPWD.c_str()
    );

    server.send( 200,"text/html","<h1>Connecting...</h1>");
    
}

void startWebServer()
{
    server.on("/", handleRoot);

    server.on("/save",handleSave);

    server.begin();
}