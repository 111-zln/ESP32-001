#pragma once 
#include "driver/gpio.h"
#include "Gpio.h"

class Button
{
public:
    explicit Button(gpio_num_t pin);
    void init();
    bool isPressed();      // 边沿检测，按一次只返回一次 true
    bool isHeld() const;   // 当前是否正按着

private:
    Gpio gpio_;
    bool lastState_;       // true=未按下(高电平)
};