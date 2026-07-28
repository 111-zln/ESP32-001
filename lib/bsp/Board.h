#pragma once

#include "../hal/Gpio.h"
#include "../hal/SpiHAL.h"
#include "../hal/I2cHAL.h"
#include "../hal/UartHAL.h"

#include "../drivers/RGB.h"
#include "../drivers/LCD.h"
#include "../drivers/Button.h"
#include "../drivers/Bme280.h"
#include "../drivers/CO2.h"

#include "../services/display_service.h"
#include "../services/sensor_service.h"

class Board
{
    public:
        Board();
        void init();
        
        
        // HAL
        Spi spi_;
        I2c i2c_;
        Uart uart_wire;
        Uart uart_ucb;


        // Drivers
        Rgb rgb_;
        Lcd lcd_;
        Bme280 bme_;
        Co2 co2_;

        Button up_;
        Button down_;
        Button left_;
        Button right_;
        Button ok_;
        Button key1_;
        Button key2_;


        // Services
        Sensor sensor_;
};