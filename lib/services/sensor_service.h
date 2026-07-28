#pragma once

#include "Bme280.h"
#include "Co2.h"
#include "data.h"
class Sensor
{
public:
    Sensor();

    void init(Bme280& bme280, Co2& co2);
    void update();

    float getTemperature();
    float getHumidity();
    float getPressure();
    int getCo2();

private:
    Bme280* bme280_;
    Co2* co2_;
};
extern Sensor sensorService ;