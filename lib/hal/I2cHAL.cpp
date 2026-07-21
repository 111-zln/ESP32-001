#include "../hal/I2cHAL.h"
#include <Wire.h>//arduino的i2c库

I2c::I2c(uint8_t sda, uint8_t scl)
    : sda_(sda),scl_(scl)
{
}

void I2c::init()
{
    Wire.begin(sda_, scl_);
}

bool I2c::write(uint8_t addr,uint8_t reg,const uint8_t* data,size_t len)
{
    Wire.beginTransmission(addr); //准备和地址xxx通信

    Wire.write(reg);              //访问寄存器reg
    Wire.write(data, len);        //发送数据及长度

    return Wire.endTransmission() == 0; //把刚才的数据真正发出去  0-成功   ！0-失败 
}

bool I2c::read(uint8_t addr,uint8_t reg,uint8_t* data,size_t len)
{
    Wire.beginTransmission(addr);

    Wire.write(reg);

    if (Wire.endTransmission(false) != 0) //把这个寄存器地址发出去，但不要结束通信，false表示不发送停止位，继续保持通信
        return false;

    if (Wire.requestFrom(addr, (uint8_t)len) != len) //向设备请求数据
        return false;

    for (size_t i = 0; i < len; ++i)
    {
        data[i] = Wire.read();   //把收到的数据取出来
    }

    return true;
}