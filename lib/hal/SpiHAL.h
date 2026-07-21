#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <driver/gpio.h>

class Spi
{
    public:
        Spi(int sck,int miso,int mosi); //绑定引脚

        void init(); //开启spi

        bool write(const uint8_t* data,size_t length); //写入数据

        bool transfer(const uint8_t* tx,uint8_t* rx,size_t length); //传输数据

    private:
        int sck_ ; 
        int miso_;
        int mosi_;
};