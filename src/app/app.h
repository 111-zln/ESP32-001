#pragma once

#include "../services/display_service.h"
#include "../services/sensor_service.h"
#include "../services/wifi_service.h"
#include "../drivers/Button.h"
#include "../bsp/Pins.hpp"
#include "../bsp/Board.h"

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