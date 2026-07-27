#include "app.h"

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

    currentPage_->draw();
}

void App::handleMainMenu()
{
    if(right_.isPressed())
    {
        g_data.menuIndex++;

        if(g_data.menuIndex > 3)
            g_data.menuIndex = 0;
    }

    if(left_.isPressed())
    {
        
    }

    if(ok_.isPressed())
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
    if(key1_.isPressed())
    {
        switch_Page(&mainmenuPage);
    }
}

void App::handleWifiPage()
{
    if(up_.isPressed())
    {
        g_data.wifiSelectIndex--;
    }

    if(down_.isPressed())
    {
        g_data.wifiSelectIndex++;
    }

    if(ok_.isPressed())
    {
        wifiService.connect(g_data.wifiSelectIndex);
    }

    if(key1_.isPressed())
    {
        switch_Page(&mainmenuPage);
    }
}

void App::handleAirPage()
{
    if(left_.isPressed())
    {
        // 温度--
    }

    if(right_.isPressed())
    {
        // 温度++
    }

    if(up_.isPressed())
    {
        // 风速+
    }

    if(down_.isPressed())
    {
        // 风速-
    }

    if(ok_.isPressed())
    {
        // 开关空调
    }

    if(key1_.isPressed())
    {
        switch_Page(&mainmenuPage);
    }
}

void App::switch_Page(Page* page)
{
    currentPage_ = page;
}