#include "app.h"

constexpr uint8_t MENU_COUNT = 4;
App app_(board_);

void App::init()
{
    currentPage_ = &mainmenuPage;

    requestRefresh();
}

void App::update()
{
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

    if(currentPage_ && needRefresh_)
    {
        currentPage_->draw();

        needRefresh_ = false;
    }
}

void App::handleMainMenu()
{
    if(board_.right_.isPressed())
    {
        g_data.menuIndex++;

        if(g_data.menuIndex >= MENU_COUNT)
            g_data.menuIndex = 0;

        requestRefresh();
    }

    if(board_.left_.isPressed())
    {
        if(g_data.menuIndex > 0)
            g_data.menuIndex--;
        else
            g_data.menuIndex = MENU_COUNT - 1;

        requestRefresh();
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
    if(wifiService.getState() == WifiState::Failed)
    {
        if(board_.ok_.isPressed())
        {
            wifiService.requestRetry();
        }

        if(board_.key1_.isPressed())
        {
            switch_Page(&mainmenuPage);
        }

    return;
    }

    if(board_.up_.isPressed())
    {
        if(g_data.wifiSelectIndex > 0)
        {
            g_data.wifiSelectIndex--;
            requestRefresh();
        }
    }

    if(board_.down_.isPressed())
    {
        if(g_data.wifiSelectIndex < g_data.wifiCount - 1)
        {
            g_data.wifiSelectIndex++;
            requestRefresh();
        }
    }

    if(board_.ok_.isPressed())
    {
        wifiService.requestStartConfig(g_data.wifiSelectIndex);
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
        {
            g_data.airSelect--;
            requestRefresh();
        }
    }

    if(board_.down_.isPressed())
    {
        if(g_data.airSelect < 2)
        {
            g_data.airSelect++;
            requestRefresh();
        }
    }

    if(board_.left_.isPressed())
    {
        switch(g_data.airSelect)
        {
        case 1:
            if(g_data.targetTemp > 16)
            {
                g_data.targetTemp--;
                requestRefresh();
            }
            break;

        case 2:
            if(g_data.fanSpeed > 0)
            {
                g_data.fanSpeed--;
                requestRefresh();
            }
            break;
        }
    }

    if(board_.right_.isPressed())
    {
        switch(g_data.airSelect)
        {
        case 1:
            if(g_data.targetTemp < 30)
            {
                g_data.targetTemp++;
                requestRefresh();
            }
            break;

        case 2:
            if(g_data.fanSpeed < 3)
            {
                g_data.fanSpeed++;
                requestRefresh();
            }
            break;
        }
    }

    if(board_.ok_.isPressed())
    {
        if(g_data.airSelect == 0)
        {
            g_data.airPower = !g_data.airPower;

            requestRefresh();
        }
    }

    if(board_.key1_.isPressed())
    {
        switch_Page(&mainmenuPage);
    }
}

void App::handleBatteryPage()
{
}

void App::switch_Page(Page* page)
{
    if(page == nullptr)
        return;

        
    if(currentPage_)
    {
        currentPage_->onExit();
    }

    currentPage_ = page;

    currentPage_->onEnter();


    requestRefresh();
}

void App::requestRefresh()
{
    needRefresh_ = true;
}