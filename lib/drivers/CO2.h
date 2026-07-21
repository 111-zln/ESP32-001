#pragma once
#include "Arduino.h"
#include "../hal/I2cHAL.h"
#include <cstdint> //提供固定长度的整数类型。

class Co2
{
    public:
        bool init(I2c& i2c); //绑定i2c

        bool start(); //开启i2c测量

        bool read(int16_t& co2, float& temp, float& humidity);  //读出co2，温度，湿度，返回值表示是否成功读取

    private:
        I2c* i2c_;

        static constexpr uint8_t ADDRESS = 0x62; // SCD4x 固定地址 static-所有对象都这个地址，constexpr-常量
};