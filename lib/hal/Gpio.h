#pragma once

#include <driver/gpio.h>

class Gpio
{
    public:

        Gpio(gpio_num_t pin);//gpio初始化

        void setOutput();//设为输出模式

        void setInput();//设为输入模式

        void write(bool level);//写入高低电平

        bool read() const;//读出高低电平

        void toggle(); //切换GPIO的电平

    private:

        gpio_num_t pin_; //(_ 表示这是成员变量)
};