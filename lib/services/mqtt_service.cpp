#include "mqtt_service.h"


//mqtt回调
void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    if(strcmp(topic,"cmd") != 0)
    {
    return;
    }

    Serial.print("收到主题：");
    Serial.println(topic);

    String msg = "";

    for(int i = 0; i < length; i++)
    {
        msg += (char)payload[i];
    }

    Serial.print("原始数据：");
    Serial.println(msg);

    JsonDocument doc;

    DeserializationError error =deserializeJson(doc, msg);//msg就是json数据例如 "temp"：28  "page": wifi 

    if(error)
    {
        Serial.println("解析失败");
        return;
    }

    const char* page = doc["page"]; 

    if(!doc["page"].is<const char*>())
    {
    return;
    }

    if(strcmp(page, "wifi") == 0)
    {
        requestPage(&wifiPage);
    }
    else if(strcmp(page, "switch") == 0)
    {
        requestPage(&switchPage);
    }
    else if(strcmp(page, "temp") == 0)
    {
        requestPage(&tempPage);
    }
    else if(strcmp(page, "battery") == 0)
    {
        requestPage(&batteryPage);
    }
    else if(strcmp(page, "menu") == 0)
    {
        requestPage(&mainmenuPage);
    }
    else
    {
        return;
    }

}
//mqtt连接一次
WiFiClient esp32client;
PubSubClient client(esp32client);
void mqtt_connect()
{
  client.setServer("broker.emqx.io",1883);
  client.setCallback(mqtt_callback);


}
//定时检测wifi和mqtt连接
void mqtt_loop()
{
   static unsigned long lastTry = 0;

    if(client.connected())
    {
        client.loop();
        return;
    }

    if(millis() - lastTry > 5000)
    {
        lastTry = millis();

        Serial.println("尝试连接MQTT");

        if(client.connect("esp32"))
        {
            client.subscribe("cmd");
        }
    }
}
void publishStatus() //心跳上报
{
    JsonDocument doc;

    doc["temp"] = g_data.temp;

    doc["battery"] = g_data.battery;

    char buffer[128]; 

    serializeJson(doc, buffer); //将doc序列化存入buffer

    client.publish("status", buffer); //发布buffer
}