#pragma once

#include "Gpio.h"
#include "SpiHAL.h"
#include "I2cHAL.h"
#include "UartHAL.h"

#include "RGB.h"
#include "LCD.h"
#include "Button.h"
#include "Bme280.h"
#include "CO2.h"

#include "display_service.h"
#include "sensor_service.h"
#include "wifi_service.h"

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
};
extern Board board_;