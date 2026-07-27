#pragma once

#include "../bsp/Board.h"

class App
{
public:
    void init(Board& board);

    void loop();

private:

    void handleButton();

    void updatePage();

    void updateSensor();

    void updateWifi();
};

