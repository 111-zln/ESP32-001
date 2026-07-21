#pragma once

#include <cstdint>
#include <cstddef>

class I2c
{
    public:

        I2c(uint8_t sda, uint8_t scl);//绑定引脚

        void init(); //开启i2c

        bool write(uint8_t address,uint8_t reg,const uint8_t* data,size_t length); //写入数据

        bool read(uint8_t address,uint8_t reg,uint8_t* data,size_t length); //读出数据

    private:
        uint8_t sda_;

        uint8_t scl_;
};