#include "app.h"

void App::init()
{
    currentPage_ = &mainmenuPage;
}

void App::update()
{
    //button.update();

    sensorService.update();

    wifiService.update();

    if(currentPage_ == &mainmenuPage)
    {
        handleMainMenu();
    }

    else if(currentPage_ == &tempPage)
    {
        handleSensorPage();
    }

    else if(currentPage_ == &wifiPage)
    {
        handleWifiPage();
    }

    else if(currentPage_ == &switchPage)
    {
        handleAirPage();
    }

    else if(currentPage_ == &batteryPage)
    {
        handleBatteryPage();
    }

    currentPage_->draw();
}