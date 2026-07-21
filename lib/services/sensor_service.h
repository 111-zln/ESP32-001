#pragma once

#include "../drivers/Bme280.h"
#include "../drivers/Co2.h"
#include "../drivers/RGB.h"

class Sensor
{
    public:

        explicit Sensor();

        void init(  Bme280 bme280 ,Co2 co2);
        


    private:
        Bme280 bme280_;
        Co2 co2_;
        Rgb rgb_;
};