#pragma once

#include "I2cHAL.h"

struct Calibration
{
    //温度补偿参数
    uint16_t dig_T1;
    int16_t   dig_T2;
    int16_t   dig_T3;

    //湿度补偿参数
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;

    //压力补偿参数
    uint16_t  dig_P1;
    int16_t   dig_P2;
    int16_t   dig_P3;
    int16_t   dig_P4;
    int16_t   dig_P5;   
    int16_t   dig_P6;
    int16_t   dig_P7;
    int16_t   dig_P8;
    int16_t   dig_P9;

};

class Bme280
{
    public:
        bool init(I2c& i2c);
        uint16_t  readhumidityRAW();//读出湿度原始值
        uint32_t  readtempRAW();
        uint32_t  readpressureRAW();

        float readhumidity(); //读出湿度
        float readtemp();
        float readpressure();

    private:
        I2c* i2c_;//这个也是要用别人的i2c对象，两阶段初始化（对象先创建，之后才知道依赖）
                  //指向 Board 创建的 I2C 对象，不负责创建和释放

        static constexpr uint8_t ADDRESS = 0x76;

        Calibration cal_;

        int32_t t_fine_;
};



