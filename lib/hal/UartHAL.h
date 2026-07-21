#pragma once

#include <Arduino.h>

class Uart
{
public:
    Uart(HardwareSerial& serial,int tx,int rx,uint32_t baud);//绑定串口，引脚和波特率

    void init(); //开启串口

    bool write(const uint8_t* data, size_t length); //写入数据

    size_t read(uint8_t* buffer, size_t length); //读出数据，返回实际读取的字节数

private:
    HardwareSerial& serial_;
    int tx_;
    int rx_;
    uint32_t baud_;
};