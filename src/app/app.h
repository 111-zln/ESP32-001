#pragma once

#include "../services/display_service.h"
#include "../services/sensor_service.h"
#include "../services/wifi_service.h"
class App
{
public:

    void init();

    void update();

private:

    Page* currentPage_;

    void handleMainMenu();

    void handleSensorPage();

    void handleWifiPage();

    void handleAirPage();

    void handleBatteryPage();
};