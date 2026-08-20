#pragma once 

#include "Gpio.h"
#include "driver/gpio.h"

class Rgb
{

    public:
        Rgb(gpio_num_t r, gpio_num_t g, gpio_num_t b);

        void red();

        void green();

        void blue();

        void off();
        
        void on();
    private:
        Gpio r_;
        Gpio g_;
        Gpio b_;
};