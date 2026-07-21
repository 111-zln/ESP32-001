#pragma once

#include "../drivers/Bme280.h"
#include "../drivers/Co2.h"
#include "../drivers/RGB.h"

class Sensor
{
    public:
        void init(  Bme280 bme280_ ,Co2 co2_);
        





    private:
        Bme280 bme280_;
        Co2 co2_;
        Rgb rgb_;
};