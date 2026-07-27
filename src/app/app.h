#pragma once

#include "../services/display_service.h"
#include "../services/sensor_service.h"
#include "../services/wifi_service.h"
#include "../drivers/Button.h"

class App
{
public:
    void init();
    void update();

private:
    // 当前页面
    Page* currentPage_;

    // 七个按键（根据你的实际GPIO修改）
    Button up_;
    Button down_;
    Button left_;
    Button right_;
    Button ok_;

    Button key1_;
    Button key2_;

    // 每个页面自己的业务
    void handleMainMenu();
    void handleSensorPage();
    void handleWifiPage();
    void handleAirPage();
    void handleBatteryPage();

    // 页面切换
    void switch_Page(Page* page);
};