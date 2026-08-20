#pragma once 
#include "driver/gpio.h"
#include "Gpio.h"

class Button
{
public:
    explicit Button(gpio_num_t pin);
    void init();
    bool isPressed();
    bool isHeld() const;
private:
    Gpio gpio_;
    gpio_num_t pin_;
    bool lastState_ = true;
};