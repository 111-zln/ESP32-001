#pragma once

#include "../drivers/Bme280.h"
#include "../drivers/Co2.h"

class Sensor
{
public:
    Sensor();

    void init(Bme280& bme280, Co2& co2);

    float getTemperature();
    float getHumidity();
    float getPressure();
    int getCo2();


    void update();

private:
    Bme280* bme280_;
    Co2* co2_;
};