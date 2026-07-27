#include "display_service.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include "../bsp/Pins.hpp"
#include "../services/sensor_service.h"

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

#define MENU_COUNT 4

Page* currentPage = nullptr;
Page* nextPage = nullptr;

MainMenuPage mainmenuPage;
BatteryPage  batteryPage ; 
TempPage     tempPage;
WifiPage     wifiPage;
SwitchPage   switchPage ;
//State currentState = MAIN_MENU;


//主菜单操作
void MainMenuPage ::draw()
{
  u8g2_.clearBuffer();

  u8g2_.setFont(u8g2_font_wqy12_t_chinese2);

  u8g2_.drawXBM( 4,0,16,16,epd_bitmap_wifi);

  u8g2_.drawXBM(24, 0,16,16,epd_bitmap_temperature_celsius);

  u8g2_.drawXBM(44,0,16,16,epd_bitmap_battery_10 );

  u8g2_.drawXBM(64,0,16,16,epd_bitmap_toggle_switch_off_outline );


  u8g2_.drawStr((20*key_num)+4,20,"^"); //生成光标

  u8g2_.sendBuffer();

}
/*
const State menuTable[] =
{
    WIFI_PAGE,
    BLUETOOTH_PAGE,
    TEMP_PAGE,
    BATTERY_PAGE,
    SWITCH_ON,
    SWITCH_OFF
};*/
/*void MainMenuPage ::handleEvent(Event event)
{
    if(event.type == EVENT_SHORT_PRESS)
    {
      key_num = (key_num + 1) % MENU_COUNT;
      needRefresh = true;
    }

    if(event.type == EVENT_LONG_PRESS)
    {
        currentState = menuTable[key_num];
                
        switch(currentState)
        {
            case WIFI_PAGE:
            requestPage(&wifiPage);
            break;

            case BLUETOOTH_PAGE:
            requestPage(&bluetoothPage);
            break;

            case TEMP_PAGE:
            requestPage(&tempPage);
            break;

            case BATTERY_PAGE:
            requestPage(&batteryPage);
            break;
                    
        }
      }	
}
*/


//电量页面操作
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
/*void BatteryPage ::handleEvent(Event event)
{
    if(event.type == EVENT_LONG_PRESS)
    {
        setCurrentPage(&mainmenuPage);
    }
}*/


//temp操作
TempPage::TempPage()
{
    editMode = false;

    Serial.println("TempPage Create");
}
void TempPage::draw()
{
    g_data.temp     = sensor_.getTemperature(); //单位DegC
    g_data.humidity = sensor_.getHumidity();    //%RH
    g_data.pressure = sensor_.getPressure();    // Pa
    g_data.co2 = sensor_.getCo2();              //ppm


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
void TempPage::addTemp()
{
    g_data.temp++;

    saveTemp(g_data.temp);

    needRefresh = true;
  
}
void TempPage::enterEdit()
{
    editMode = true;
    
}
void TempPage::exitEdit()
{
    editMode = false;
}
bool TempPage::isEditMode()
{
    return editMode;
}
/*void TempPage::handleEvent(Event event)
{
    if(!editMode)
    {
        if(event.type==EVENT_SHORT_PRESS)
        {
            enterEdit();
        }

        else if(event.type==EVENT_LONG_PRESS)
        {

            setCurrentPage(&mainmenuPage);
            needRefresh = true;
        }
    }
    else
    {
        if(event.type==EVENT_SHORT_PRESS)
        {
            addTemp();
        }

        else if(event.type==EVENT_LONG_PRESS)
        {
            exitEdit();
        }
    }
}*/
void TempPage::onEnter()
{
    Serial.println("Temp Enter");
}
void TempPage::onExit()
{
    Serial.println("Temp Exit");
}

//wifi操作
void WifiPage::draw()
{
    u8g2_.clearBuffer();
    u8g2_.setFont(u8g2_font_5x8_tf);

    // 浏览状态
    if(!wifiConnecting && !wifiConnected)
    {
        for(int i=0;i<min(g_data.wifiCount,wifi_maxnum);i++)
        {
            int y = 4+i*10;

            if(i==selectIndex)
            {
                u8g2_.drawStr(4,y,">");
            }

            u8g2_.drawStr(10,y,g_data.wifiList[i].c_str());
        }
    }

    // 连接中
    else if(wifiConnecting)
    {
        u8g2_.drawStr(20,20,"Connecting...");
        u8g2_.drawStr(20,35,selectedSSID.c_str());
        u8g2_.drawStr(10,55,"Long Press Back");
    }

    // 已连接
    else if(wifiConnected)
    {
        u8g2_.drawStr(20,20,"Connected");
        u8g2_.drawStr(20,35,selectedSSID.c_str());
        u8g2_.drawStr(10,55,"Long Press Back");
    }

    u8g2_.sendBuffer();
    
}
/*void WifiPage::handleEvent(Event event)
{
    if(event.type == EVENT_SHORT_PRESS)
    {
        selectIndex++;

        if(selectIndex>=min(g_data.wifiCount,wifi_maxnum))
        {
            selectIndex=0;
        }

        needRefresh=true;
    }
    if(event.type == EVENT_LONG_PRESS)
    {
        //浏览列表 ->开始连接
         if(!wifiConnecting && !wifiConnected)
        {
            needRefresh = true;
            wifiConnecting = true;
            wifiConnected = false;

            selectedSSID = g_data.wifiList[selectIndex];

            Serial.println("选择:");
            Serial.println(selectedSSID);

            Serial.println("请连接热点");
            Serial.println("ESP32_Remoter");
            Serial.println("打开192.168.4.1");
        }

        // 正在连接 → 返回主菜单
        else if(wifiConnecting)
        {
            wifiConnecting = false;
            setCurrentPage(&mainmenuPage);
        }

        // 已连接成功 → 返回主菜单
        else if(wifiConnected)
        {
            setCurrentPage(&mainmenuPage);
        }

    }
}

void WifiPage::onEnter()
{
    scanWifi();
    
    needRefresh=true;
}

void WifiPage::onExit() 
{
    wifiNeedScan = true;
}*/


//开关操作
void SwitchPage::draw()
{
    u8g2_.clearBuffer();

    u8g2_.setFont(u8g2_font_wqy12_t_chinese2);
    u8g2_.drawStr(4,0,"switch");
    u8g2_.drawStr(4,20,"ON");
    u8g2_.drawStr(4,40,"Long Press Back");

    u8g2_.sendBuffer();
}
/*void SwitchPage::handleEvent(Event event)
{
    if(event.type == EVENT_LONG_PRESS)
    {

        setCurrentPage(&mainmenuPage);
    }
}*/


//页面切换
void setCurrentPage(Page* page)
{
    if(currentPage)
    {
        currentPage->onExit();
    }

    currentPage = page;

    if(currentPage)
    {
        currentPage->onEnter();
    }

     needRefresh = true;   // ← 加这里
}
void requestPage(Page* page)
{
    nextPage = page;
}