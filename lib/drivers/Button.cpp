#include "Button.h"
#include <Arduino.h>

Button::Button(gpio_num_t pin): gpio_(pin), pin_(pin), lastState_(true)
{
    // 全局构造阶段不做硬件操作
}

void Button::init()
{
    pinMode((uint8_t)pin_, INPUT);   // 外接下拉电阻
    lastState_ = (digitalRead((uint8_t)pin_) == LOW);
}

bool Button::isPressed()
{
    bool currentPressed = (digitalRead((uint8_t)pin_) == HIGH);  // 按下接VCC
    bool pressed = (currentPressed && lastState_);
    lastState_ = !currentPressed;
    return pressed;
}

bool Button::isHeld() const
{
    return (digitalRead((uint8_t)pin_) == HIGH);
}