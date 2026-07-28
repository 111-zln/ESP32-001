#pragma once 

#include "driver/gpio.h"
#include "Gpio.h"

class Button
{
    
    public:

        explicit Button(gpio_num_t pin);

        void init();     //设为输出模式

        bool isPressed() const;  //检测是否按压 



    private:
        Gpio gpio_; //创建一个属于自己的对象，但是这个对象要使用外部传入的pin，所以要在构造函数中传入一个pin
};