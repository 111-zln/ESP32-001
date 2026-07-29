#pragma once

#include "display_service.h"
#include "sensor_service.h"
#include "wifi_service.h"
#include "Button.h"
#include "Pins.hpp"
#include "Board.h"
#include "tasks.h"

class App
{
public:
     App(Board& board)
        : board_(board)
    {}

    void init();
    void update();

private:
    // 当前页面
    Page* currentPage_;

    Board& board_;

    // 每个页面自己的业务
    void handleMainMenu();
    void handleSensorPage();
    void handleWifiPage();
    void handleAirPage();
    void handleBatteryPage();

    // 页面切换
    void switch_Page(Page* page);
};
extern App app_ ;