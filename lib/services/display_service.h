#pragma once

#include "../drivers/LCD.h"
#include "../bsp/data.h"

//页面跳转接口
class Page
{
    public:
        virtual void onEnter(){}

        virtual void onExit(){}
        
        virtual void draw() = 0;

        virtual void enter(){}

        virtual void exit(){}

        //virtual void handleEvent(Event event){}
};
extern Page* currentPage;
extern Page* nextPage;

void requestPage(Page* page);
void setCurrentPage(Page* page);


//主菜单
class MainMenuPage :public Page
{
    public:

        void draw() override ;
        //void handleEvent(Event event) override ;
};
extern MainMenuPage mainmenuPage;


//电量
class BatteryPage : public Page
{
    public:

        void draw() override;
        //void handleEvent(Event event) override ;
};
extern BatteryPage batteryPage;


//temp
class TempPage : public Page
{
    private:

        bool editMode ;
        

    public:

        TempPage();

        void onEnter() override;

        void onExit() override;

        void draw() override;//我是在重写 Page 里面的 draw()

        void enterEdit();

        void exitEdit();

        bool isEditMode();

        void addTemp();    

        //void handleEvent(Event event) override ;

};
extern TempPage tempPage;


//wifi
class WifiPage : public Page
{
    private:
        int selectIndex = 0;
        
    public:

        void draw() override;

        //void handleEvent(Event event) override ;
        
        void onEnter() override;
 
        void onExit() override;
};
extern WifiPage wifiPage;


//switch
class SwitchPage : public Page
{
    public:

        void draw() override;
        //void handleEvent(Event event) override ;
};
extern SwitchPage  switchPage ;