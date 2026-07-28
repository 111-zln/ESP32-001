#include "Button.h"
#include <Arduino.h>

Button::Button(gpio_num_t pin): gpio_(pin)
{
    gpio_.setInput();

    gpio_pullup_en(pin);
}

bool Button::isPressed() const
{
    if(gpio_.read() == 0)
    {
        delay(10);

        return gpio_.read() == 0;
    }

    return false;
}