#include "Button.h"
#include <Arduino.h>

Button::Button(gpio_num_t pin)
    : gpio_(pin), lastState_(true)
{
    gpio_.setInput();
    gpio_pullup_en(pin);
}

void Button::init()
{
    // 同步初始状态：高电平=未按下
    lastState_ = (gpio_.read() != 0);
}

bool Button::isPressed()
{
    bool current = (gpio_.read() == 0);   // 0 = 按下（上拉）
    bool pressed = (current && lastState_);
    lastState_ = !current;                 // true=未按下, false=按下
    return pressed;
}

bool Button::isHeld() const
{
    return (gpio_.read() == 0);
}