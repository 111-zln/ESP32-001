#include "display_service.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include "../bsp/Pins.hpp"
#include "../services/sensor_service.h"
#include "../services/wifi_service.h"

// '电池', 16x16px
const unsigned char epd_bitmap_battery_10 [] PROGMEM = {
	0xff, 0xff, 0x3f, 0xfc, 0x3f, 0xfc, 0x0f, 0xf0, 0xef, 0xf7, 0xef, 0xf7, 0xef, 0xf7, 0xef, 0xf7, 
	0xef, 0xf7, 0xef, 0xf7, 0xef, 0xf7, 0xef, 0xf7, 0x0f, 0xf0, 0x0f, 0xf0, 0x1f, 0xf8, 0xff, 0xff
};
// '温度', 16x16px
const unsigned char epd_bitmap_temperature_celsius [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xc3, 0xf3, 0xc3, 0xc0, 0x67, 0xec, 0x3f, 0xfe, 0x3f, 0xff, 
	0x3f, 0xff, 0x3f, 0xff, 0x7f, 0xfe, 0x7f, 0xe0, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
// 'wifi', 16x16px
const unsigned char epd_bitmap_wifi [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0x1f, 0xf8, 0x03, 0xc0, 0xe1, 0x87, 0xfb, 0xdf, 0x1f, 0xf8, 0x0f, 0xf0, 
	0xcf, 0xf3, 0xff, 0xff, 0x3f, 0xfc, 0x3f, 0xfc, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
// '开', 16x16px
const unsigned char epd_bitmap_toggle_switch [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xc0, 0x3b, 0x80, 0x79, 
	0x80, 0x79, 0xc0, 0x3b, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
// '关', 16x16px
const unsigned char epd_bitmap_toggle_switch_off_outline [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0xcf, 0xf3, 0x83, 0xf9, 0xa1, 0xfd, 
	0xa1, 0xfd, 0x83, 0xf9, 0xcf, 0xf3, 0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const unsigned char* epd_bitmap_allArray[6] = {
	epd_bitmap_battery_10, //后面要换成风扇（风速）
	epd_bitmap_temperature_celsius,
	epd_bitmap_wifi,
    epd_bitmap_toggle_switch, //空调开启
	epd_bitmap_toggle_switch_off_outline//空调关闭
};
const int epd_bitmap_allArray_LEN = 5;

MainMenuPage mainmenuPage;
BatteryPage  batteryPage ; 
TempPage     tempPage;
WifiPage     wifiPage;
SwitchPage   switchPage ;


//主菜单
void MainMenuPage ::draw()
{
  u8g2_.clearBuffer();

  u8g2_.setFont(u8g2_font_wqy12_t_chinese2);

  u8g2_.drawXBM( 4,0,16,16,epd_bitmap_wifi);

  u8g2_.drawXBM(24, 0,16,16,epd_bitmap_temperature_celsius);

  u8g2_.drawXBM(44,0,16,16,epd_bitmap_battery_10 );

  u8g2_.drawXBM(64,0,16,16,epd_bitmap_toggle_switch_off_outline );


  u8g2_.drawStr((20*g_data.menuIndex)+4,20,"^"); //生成光标

  u8g2_.sendBuffer();

}


//电量页面
void BatteryPage ::draw()
{
    u8g2_.clearBuffer();

    u8g2_.setFont(u8g2_font_wqy12_t_chinese2);
    u8g2_.drawStr(4,0,"Battery");

    char buf[16];
    sprintf(buf,"%d%%",g_data.battery);
    u8g2_.drawStr(4,20,buf);
    u8g2_.drawStr(4,40,"Long Press Back");

    u8g2_.sendBuffer();
}


//temp
TempPage::TempPage()
{
    editMode = false;

    Serial.println("TempPage Create");
}

void TempPage::draw()
{

    u8g2_.clearBuffer();

    u8g2_.setFont(u8g2_font_6x12_tf);

    u8g2_.setCursor(4, 12);
    u8g2_.print("Temp: ");
    u8g2_.print(g_data.temp);
    u8g2_.print(" C");

    u8g2_.setCursor(4,26);
    u8g2_.print("Humi: ");
    u8g2_.print(g_data.humidity);
    u8g2_.print(" %");

    u8g2_.setCursor(4,40);
    u8g2_.print("Pres: ");
    u8g2_.print(g_data.pressure);
    u8g2_.print(" Pa");

    u8g2_.setCursor(4,54);
    u8g2_.print("CO2 : ");
    u8g2_.print(g_data.co2);
    u8g2_.print(" ppm");

   

    if(editMode)
    {
        u8g2_.setFont(u8g2_font_4x6_tf);
        u8g2_.setCursor(70,45);
        u8g2_.print("Edit");
    }

    u8g2_.sendBuffer();


}

void TempPage::onEnter()
{
    Serial.println("Temp Enter");
}

void TempPage::onExit()
{
    Serial.println("Temp Exit");
}


//wifi
void WifiPage::draw()
{
    u8g2_.clearBuffer();
    u8g2_.setFont(u8g2_font_5x8_tf);


    switch(g_data.wifiState)
    {
    case WIFI_LIST:
        drawWifiList();
        break;

    case WIFI_CONNECTING:
        drawConnecting();
        break;

    case WIFI_CONNECTED:
        drawConnected();
        break;
    }

    u8g2_.sendBuffer();
    
}

void WifiPage::drawWifiList()
{
    constexpr int MAX_SHOW = 5;

    for(int i = 0; i < min(g_data.wifiCount, MAX_SHOW); i++)
    {
        int y = 4 + i * 10;

        if(i == g_data.wifiSelectIndex)
            u8g2_.drawStr(4, y, ">");

        u8g2_.drawStr(10, y, g_data.wifiList[i].c_str());

    }
}

void WifiPage::drawConnecting()
{
    u8g2_.drawStr(4, 8,  "AP:");
    String AP_id = wifiService.AP_ID;
    u8g2_.drawStr(4, 18,AP_id.c_str());

    u8g2_.drawStr(4, 30, "PWD:");
    String AP_pwd = wifiService.AP_PWD;
    u8g2_.drawStr(4, 40, AP_pwd.c_str());

    u8g2_.drawStr(4, 52, "OPEN Web:");
    String ip = wifiService.getApIP();
    u8g2_.drawStr(34, 52, ip.c_str());

    u8g2_.drawStr(4, 62, "WiFi PWD->");
}

void WifiPage::drawConnected()
{
    u8g2_.drawStr(20,20,"Connected");
    u8g2_.drawStr(20,35,g_data.savedSSID.c_str());
}


//开关操作
void SwitchPage::draw()
{
    u8g2_.clearBuffer();
    u8g2_.setFont(u8g2_font_5x8_tf);

    char buf[24];

    // 标题
    u8g2_.drawStr(28, 8, "AIR CONTROL");
    u8g2_.drawHLine(0, 11, 128);

    // 光标
    switch (g_data.airSelect)
    {
    case 0:
        u8g2_.drawStr(0, 24, ">");
        break;
    case 1:
        u8g2_.drawStr(0, 36, ">");
        break;
    case 2:
        u8g2_.drawStr(0, 48, ">");
        break;
    }

    // Power
    sprintf(buf, " Power : %s",
            g_data.airPower ? "ON" : "OFF");
    u8g2_.drawStr(4, 24, buf);

    // Temp
    sprintf(buf, " Temp  : %d C",
            g_data.targetTemp);
    u8g2_.drawStr(4, 36, buf);

    // Fan
    const char *fanStr[] =
    {
        "LOW",
        "MID",
        "HIGH",
        "AUTO"
    };

    sprintf(buf, " Fan   : %s",
            fanStr[g_data.fanSpeed]);

    u8g2_.drawStr(4, 48, buf);

    // 底部提示
    u8g2_.drawHLine(0, 54, 128);
    u8g2_.drawStr(4, 62, "KEY1:Back");

    u8g2_.sendBuffer();
}