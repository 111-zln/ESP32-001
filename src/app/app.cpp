#include "app.h"

constexpr uint8_t MENU_COUNT = 4;
App app_;

void App::init()
{
    currentPage_ = &mainmenuPage;
}

void App::update()
{
   
    // 更新后台功能
    sensorService.update();
    wifiService.update();

    // 当前页面处理按键
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

      if(currentPage_)
    {
        currentPage_->draw();
    }
}

void App::handleMainMenu()
{
    if(board_.right_.isPressed())
    {
        g_data.menuIndex++;

        if(g_data.menuIndex >= MENU_COUNT)
            g_data.menuIndex = 0;
    }

    if(board_.left_.isPressed())
    {
        
    }

    if(board_.ok_.isPressed())
    {
        switch(g_data.menuIndex)
        {
        case 0:
            switch_Page(&wifiPage);
            break;

        case 1:
            switch_Page(&tempPage);
            break;

        case 2:
            switch_Page(&batteryPage);
            break;

        case 3:
            switch_Page(&switchPage);
            break;
        }
    }
}

void App::handleSensorPage()
{
    if(board_.key1_.isPressed())
    {
        switch_Page(&mainmenuPage);
    }
}

void App::handleWifiPage()
{
    if(board_.up_.isPressed())
    {
        if(g_data.wifiSelectIndex > 0)
        {
            g_data.wifiSelectIndex--;
        }
    }

    if(board_.down_.isPressed())
    {
        if(g_data.wifiSelectIndex < g_data.wifiCount - 1)
        {
            g_data.wifiSelectIndex++;
        }
    }

    if(board_.ok_.isPressed())
    {
        wifiService.startConfig(g_data.wifiSelectIndex);
    }

    if(board_.key1_.isPressed())
    {
        switch_Page(&mainmenuPage);
    }
}

void App::handleAirPage()
{
    if(board_.up_.isPressed())
    {
        if(g_data.airSelect > 0)
            g_data.airSelect--;
    }

    if(board_.down_.isPressed())
    {
        if(g_data.airSelect < 2)
            g_data.airSelect++;
    }

    if(board_.left_.isPressed())
    {
        switch(g_data.airSelect)
        {
        case 1:     // Temp
            if(g_data.targetTemp > 16)
                g_data.targetTemp--;
            break;

        case 2:     // Fan
            if(g_data.fanSpeed > 0)
                g_data.fanSpeed--;
            break;
        }
    }

    if(board_.right_.isPressed())
    {
        switch(g_data.airSelect)
        {
        case 1:
            if(g_data.targetTemp < 30)
                g_data.targetTemp++;
            break;

        case 2:
            if(g_data.fanSpeed < 3)
                g_data.fanSpeed++;
            break;
        }
    }

    if(board_.ok_.isPressed())
    {
        if(g_data.airSelect == 0)
        {
            g_data.airPower = !g_data.airPower;
        }
    }

    if(board_.key1_.isPressed())
    {
        switch_Page(&mainmenuPage);
    }
}

void App::switch_Page(Page* page)
{
    if(page != nullptr)
    {
        currentPage_ = page;
    }
}