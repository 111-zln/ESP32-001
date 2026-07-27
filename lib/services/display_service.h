#pragma once

#include "../drivers/LCD.h"
#include "../bsp/data.h"



// 页面基类
class Page
{
public:
    virtual ~Page() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void draw() = 0;
};

// 主菜单
class MainMenuPage : public Page
{
public:
    void draw() override;
};
extern MainMenuPage mainmenuPage;

// Battery
class BatteryPage : public Page
{
public:
    void draw() override;
};
extern BatteryPage batteryPage;


// Sensor
class TempPage : public Page
{
public:
    TempPage();

    void onEnter() override;
    void onExit() override;

    void draw() override;

private:
    bool editMode = false;
};
extern TempPage tempPage;

// Wifi
class WifiPage : public Page
{
public:
    void draw() override;
    void drawWifiList();
    void drawConnecting();
    void drawConnected();


private:
    int selectIndex = 0;
};
extern WifiPage wifiPage;


// Air Switch
class SwitchPage : public Page
{
public:
    void draw() override;
};
extern SwitchPage switchPage;